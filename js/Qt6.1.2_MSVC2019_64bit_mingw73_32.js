//-----------------------------------------------------------------------------------------
let binGroupIndex = 0;
let pluginsGroupIndex = 0;
let qmlGroupIndex = 0;
let resourceGroupIndex = 0;
let msvcGroupIndex = 0;
let mingwGroupIndex = 0;
let dataBaseGroupIndex = 0;
let sslGroupIndex = 0;
let systemRootGroupIndex = 0;
let otherGroupIndex = 0;
let systemRootIndex = 0;
let exeDirIndex = 0;
//-----------------------------------------------------------------------------------------
function init()
{
	tree.clearGroups();

	binGroupIndex = tree.addGroup("bin");
	pluginsGroupIndex = tree.addGroup("plugins");
	qmlGroupIndex = tree.addGroup("qml");
	resourceGroupIndex = tree.addGroup("resources");
	msvcGroupIndex = tree.addGroup("msvc");
	mingwGroupIndex = tree.addGroup("mingw");	
	exeDirIndex = tree.addGroup("currentDir");
	dataBaseGroupIndex= tree.addGroup("databases");
	sslGroupIndex = tree.addGroup("ssl");
	systemRootGroupIndex = tree.addGroup("system");
	otherGroupIndex = tree.addGroup("other");	
}
//-----------------------------------------------------------------------------------------
function update(libs)
{
	tree.clearLibs();
		
	let systemRoot = "C:/Windows";
	for (let i in libs)
	{
		if (utils.isSubPath(QTDIR + '/bin', libs[i]))
		{
		    if(isMINGWlib(libs[i]))
			  tree.addLib(mingwGroupIndex, libs[i],true)	
			else
			{
			  tree.addLib(binGroupIndex, libs[i], true);
			  if (isWebEngineLib(libs[i]))
			  {
				tree.addLib(binGroupIndex, QTDIR + '/bin/QtWebEngineProcess.exe', true);
				let dir = new Dir(QTDIR + '/resources')
				files = dir.entryList1(Dir.Files);
				for (let i in files)
					tree.addLib(resourceGroupIndex, QTDIR + '/resources' + '/' + files[i], true);
				tree.expandGroup(resourceGroupIndex);
			  }
			}
		}
		else if(utils.isSubPath(absolutePath(FILEPATH), libs[i]))
		{
			tree.addLib(exeDirIndex, libs[i], true); 
		}
		else if (utils.isSubPath(QTDIR + '/plugins', libs[i]))
		{
			tree.addLib(pluginsGroupIndex, libs[i], true);
		}
		else if (utils.isSubPath(QTDIR + '/qml', libs[i]))
		{
			tree.addLib(qmlGroupIndex, libs[i], true);
			let qmlDirFile = absolutePath(libs[i]) + "/qmldir";
			if (fileExists(qmlDirFile))
				tree.addLib(qmlGroupIndex, qmlDirFile, true);
		}
		else if (isMSVClib(libs[i]))
			tree.addLib(msvcGroupIndex, libs[i],true)	
		else if (isMySQL(libs[i]) || isPostgreSQL(libs[i]))
			tree.addLib(dataBaseGroupIndex, libs[i],true);
        else if (isSSL(libs[i]))
			tree.addLib(sslGroupIndex, libs[i],true);      
	    else if (utils.isSubPath(systemRoot, libs[i]))
			tree.addLib(systemRootGroupIndex, libs[i]);
		else
			tree.addLib(otherGroupIndex, libs[i]);
	}

	tree.expandGroup(binGroupIndex);
	tree.expandGroup(pluginsGroupIndex);
	tree.expandGroup(qmlGroupIndex);
	tree.expandGroup(msvcGroupIndex);
	tree.expandGroup(mingwGroupIndex);
	tree.expandGroup(exeDirIndex);
	// tree.expandGroup(systemRootGroupIndex);
	// tree.expandGroup(otherGroupIndex);

	tree.hideEmptyGroups();
}
//-----------------------------------------------------------------------------------------
function copy(toDir)
{
	for (let groupIndex = 0; groupIndex < tree.groupCount(); ++groupIndex)
		for (let libIndex = 0; libIndex < tree.libCount(groupIndex); ++libIndex)
			if (tree.libIsChecked(groupIndex, libIndex))
			{
				let groupName = tree.groupName(groupIndex);
				let libName = tree.libName(groupIndex, libIndex);
				copyLib(groupName, libName, toDir);
			}
}
//-----------------------------------------------------------------------------------------
function copyLib(groupName, libName, toDir)
{
	// Определение относительно какой папки копировать
	let relDir = '';
	if (groupName == 'plugins' ||
	    groupName == 'qml' ||
	    groupName == 'resources')
		relDir = QTDIR;

	// Копирование библиотеки
	let newFileName = utils.copyFile(libName, toDir, relDir);
	if(!newFileName)
	{
		log.addError('Error: Can`t copy file "' + libName + '" to "' + toDir + '"');
		return;
	}

	// Патчинг Qt5Core, изменение вшитой переменной qt_prfxpath=.
	// http://www.riuson.com/blog/post/qtcore-hard-coded-paths
	// if(isQtCoreLib(libName))
	//{
	//	if (!utils.patchFile(newFileName, "qt_prfxpath=", "."))
	//		log.addError('Can not patch "' + newFileName + '"');
	//}

	if(os=="linux")
		utils.makeSymLinks(libFilePath,newFilePath);
}
//-----------------------------------------------------------------------------------------
//    Вспомогательные ф-ции
//-----------------------------------------------------------------------------------------
function mkDir(dirName)
{
	dir = new Dir(dirName);
	if (!dir.exists())
		dir.mkpath(dirName);
}

function fileName(file)
{
	return (new FileInfo(file)).fileName();
}

function baseName(file) // filename without dir
{
	return (new FileInfo(file)).baseName();
}

function fileExists(file)
{
	return (new FileInfo(file)).exists();
}

function absolutePath(file) // extrat dir from filepath
{
	return (new FileInfo(file)).absolutePath();
}

function isMSVClib(libName)
{
	/*
	http://doc.qt.io/qt-5/windows-deployment.html
	VC++ 12.0 (2013)	VC++ 14.0 (2015)
	The C run-time	    msvcr120.dll	    vccorlib140.dll, vcruntime140.dll
	The C++ run-time	msvcp120.dll	    msvcp140.dll
	 */
	libName = baseName(libName);
	return libName.match(/msvc[rp]\d+D?/i) ||
	       libName.match(/vccorlib\d+D?/i) ||
	       libName.match(/vcruntime\d+D?/i)
}

function isMINGWlib(libName)
{
	/*
	  https://doc.qt.io/qt-5/windows-deployment.html
  	        MinGW
      LIBWINPTHREAD-1.DLL
      LIBGCC_S_DW2-1.DLL
      LIBSTDC++-6.DLL
   */
	
	libName = baseName(libName);
	return libName.match(/libwinpthread-\d+D?/i) ||	
	       libName.match(/libgcc_s_dw\d+-\d+D?/i) ||	       
	       libName.match(/libstdc\+\+-\d+D?/i)
}

function isMySQL(libName)
{
	return baseName(libName).match(/libmysql\d*/i) // libmysql.dll
}


function isPostgreSQL(libName)
{
    /*
	LIBPQ.dll
	libintl-8.dll
	libiconv-2.dll
   */
	libName = baseName(libName);
	return libName.match(/libpq\d*?/i) ||	
	       libName.match(/libintl-\d+D?/i) ||	       
	       libName.match(/libiconv-\d+D?/i)
}

function isSSL(libName)
{
	return baseName(libName).match(/libeay|libssl|ssleay|libcrypto\d*/i) // libeay32.dll  libssl32.dll ssleay32.dll libssl-1_1-x64.dll libcrypto-1_1-x64.dll
}

function isQtCoreLib(libName)
{
	return baseName(libName).match(/Qt\d*CoreD?/i) // Qt5Core.dll
}

function isWebEngineLib(libName)
{
	return baseName(libName).match(/Qt\d+WebEngineCore.*?/i); // Qt5WebEngineCore.dll Qt5WebEngineWidgets.dll
}