/*
QTDIR  - path to Qt, is global variable.

class Log
{
   void addInfo(const QString& text, QString color="black");
   void addError(const QString& text);
   // ...
} log;

class Tree
{
public:
   int addGroup(const QString &groupName);
   void addLib(int groupIndex, const QString &libName, bool checked= false);
   void expandGroup(int groupIndex);
   int groupCount()const;
   int libCount(int groupIndex)const;
   QString groupName(int groupIndex)const;
   QString libName(int groupIndex, int libIndex)const;
   bool libIsChecked(int groupIndex,int libIndex)const;
   // ...
} tree;

class Utils
{
public:
   static bool isSubPath(const QString &dir, const QString &path);
   static QString copyFile(const QString &filePath, const QString &destinationDir, const QString &baseDir= QString());
   static bool patchFile(const QString& fileName,const QByteArray& valueName, const QByteArray& newValue);
} utils;


Dir - > QDir
FileInfo -> QFileInfo

*/


function update(libs) 
{   
		var binGroupIndex = tree.addGroup("%QTDIR%/bin");
		var pluginsGroupIndex = tree.addGroup("%QTDIR%/plugins");
		var qmlGroupIndex = tree.addGroup("%QTDIR%/qml");
    var resourceGroupIndex = tree.addGroup("%QTDIR%/resources");
           
		var systemRootGroupIndex= tree.addGroup("SYSTEMROOT");
		var otherGroupIndex = tree.addGroup("Other");

		var systemRoot = "C:/Windows";
    
		for (var i in libs) 
    {    
			if(utils.isSubPath(QTDIR + '/bin', libs[i]) )
      {
				tree.addLib(binGroupIndex, libs[i], true);
        if(isWebEngineLib(libs[i]))   
        {           
           tree.addLib(binGroupIndex, QTDIR+'/bin/QtWebEngineProcess.exe', true);        // !!! ??? Qt5WebEngineProcess
           var dir = new Dir(QTDIR+'/resources')
           files = dir.entryList1(Dir.Files);
           for(var i in files)
              tree.addLib(resourceGroupIndex, QTDIR+'/resources'+'/'+files[i], true);
           tree.expandGroup(resourceGroupIndex);
        }   
      }  
			else if(utils.isSubPath(QTDIR + '/plugins', libs[i]))
      {
				tree.addLib(pluginsGroupIndex, libs[i], true);
      }  
			else if (utils.isSubPath(QTDIR + '/qml', libs[i]))
      {
				tree.addLib(qmlGroupIndex, libs[i], true);
        var qmlDirFile = absolutePath(lib)+"/qmldir";
        if(fileExists(qmlDirFile))
          tree.addLib(qmlGroupIndex,qmlDirFile, true);                
      }  
			else if(utils.isSubPath(systemRoot,libs[i]))
			  tree.addLib(systemRootGroupIndex,libs[i],isMSVClib(libs[i]) || isMySQL(libs[i]));
			else
				tree.addLib(otherGroupIndex, libs[i], isMSVClib(libs[i]) || isMySQL(libs[i]));
		}
   
		tree.expandGroup(binGroupIndex);
		tree.expandGroup(pluginsGroupIndex);
		tree.expandGroup(qmlGroupIndex);
    tree.expandGroup(systemRootGroupIndex);
		tree.expandGroup(otherGroupIndex);
}


function copy(toDir) 
{     
    for(var groupIndex=0; groupIndex<tree.groupCount(); ++groupIndex) 
       for(var libIndex=0; libIndex<tree.libCount(groupIndex); ++libIndex)   
         if(tree.libIsChecked(groupIndex,libIndex)) 
         {
            var groupName = tree.groupName(groupIndex);
            var libName = tree.libName(groupIndex,libIndex);
            copyLib(groupName,libName,toDir);            
         }     
}

function copyLib(groupName,libName,toDir)
{  
  // Определение относительно какой папки копировать
  var relDir='';
  if(groupName=='%QTDIR%/plugins' || groupName=='%QTDIR%/qml' || groupName=='%QTDIR%/resources')
     relDir = QTDIR;   
   
  // Копирование библиотеки
  var newFileName= utils.copyFile(libName,toDir,relDir);  
  if(!newFileName)
  {
    log.addError('Error: Can`t copy file "'+libName+'" to "'+toDir+'"');  
    return;
  }  
  
  // Патчинг Qt5Core, хардкор пути qt_prfxpath=.
  if(isQtCoreLib(libName))
  {
    if(!utils.patchFile(newFileName,"qt_prfxpath=","."))
      log.addError('Не удалось пропатчить "'+newFileName+'"');
  }
  
//        #ifdef Q_OS_LINUX
//          Lc::makeSymLinks(libFilePath,newFilePath);
//        #endif   
}


function mkDir(dirName)
{
  dir = new Dir(dirName);
  if(!dir.exists())
    dir.mkpath(dirName);
}

function fileName(file)
{
  return (new FileInfo(file)).fileName();
}

function baseName(file)
{
  return (new FileInfo(file)).baseName();
}

function fileExists(file)
{
  return (new FileInfo(file)).exists();
}

function absolutePath(file)
{ 
  return (new FileInfo(file)).absolutePath();
}

function isMSVClib(libName) 
{
	/*
	http://doc.qt.io/qt-5/windows-deployment.html
	VC++ 12.0 (2013)	VC++ 14.0 (2015)
	The C run-time	  msvcr120.dll	    vccorlib140.dll, vcruntime140.dll
	The C++ run-time	msvcp120.dll	    msvcp140.dll
	 */
  libName = baseName(libName);
	return libName.match(/msvc[rp]\d+D?/i) ||
	       libName.match(/vccorlib\d+D?/i) ||
	       libName.match(/vcruntime\d+D?/i)
}

function isMySQL(libName) 
{
	return baseName(libName).match(/libmysql\d*/i) // libmysql.dll
}
function isQtCoreLib(libName) 
{
	return baseName(libName).match(/Qt\d*CoreD?/i) // Qt5Core.dll
}

function isWebEngineLib(libName)
{
  return baseName(libName).match(/Qt\d+WebEngineCore.*?/i);  // Qt5WebEngineCore.dll Qt5WebEngineWidgets.dll
} 
