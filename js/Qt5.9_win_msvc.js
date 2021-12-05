//-----------------------------------------------------------------------------------------
var binGroupIndex = 0;
var pluginsGroupIndex = 0;
var qmlGroupIndex = 0;
var resourceGroupIndex = 0;
var msvcGroupIndex = 0;
var dataBaseGroupIndex = 0;
var sslGroupIndex = 0;
var systemRootGroupIndex = 0;
var otherGroupIndex = 0;
var systemRoot = 0;
//-----------------------------------------------------------------------------------------
function init()
{
	tree.clearGroups();

	binGroupIndex = tree.addGroup("bin");
	pluginsGroupIndex = tree.addGroup("plugins");
	qmlGroupIndex = tree.addGroup("qml");
	resourceGroupIndex = tree.addGroup("resources");
	msvcGroupIndex = tree.addGroup("msvc");
	dataBaseGroupIndex= tree.addGroup("databases");
	sslGroupIndex = tree.addGroup("ssl");
	systemRootGroupIndex = tree.addGroup("system");
	otherGroupIndex = tree.addGroup("other");
}
//-----------------------------------------------------------------------------------------
function update(libs)
{
	tree.clearLibs();

	var systemRoot = "C:/Windows";
	for (var i in libs)
	{
		if (utils.isSubPath(QTDIR + '/bin', libs[i]))
		{
			tree.addLib(binGroupIndex, libs[i], true);
			if (isWebEngineLib(libs[i]))
			{
				tree.addLib(binGroupIndex, QTDIR + '/bin/QtWebEngineProcess.exe', true);
				var dir = new Dir(QTDIR + '/resources')
				files = dir.entryList1(Dir.Files);
				for (var i in files)
					tree.addLib(resourceGroupIndex, QTDIR + '/resources' + '/' + files[i], true);
				tree.expandGroup(resourceGroupIndex);
			}
		}
		else if (utils.isSubPath(QTDIR + '/plugins', libs[i]))
		{
			tree.addLib(pluginsGroupIndex, libs[i], true);
		}
		else if (utils.isSubPath(QTDIR + '/qml', libs[i]))
		{
			tree.addLib(qmlGroupIndex, libs[i], true);
			var qmlDirFile = absolutePath(libs[i]) + "/qmldir";
			if (fileExists(qmlDirFile))
				tree.addLib(qmlGroupIndex, qmlDirFile, true);
		}
		else if (isMSVClib(libs[i]))
			tree.addLib(msvcGroupIndex, libs[i],true)
		else if (isMySQL(libs[i]))
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
	// tree.expandGroup(systemRootGroupIndex);
	// tree.expandGroup(otherGroupIndex);

	tree.hideEmptyGroups();
}
//-----------------------------------------------------------------------------------------
function copy(toDir)
{
	for (var groupIndex = 0; groupIndex < tree.groupCount(); ++groupIndex)
		for (var libIndex = 0; libIndex < tree.libCount(groupIndex); ++libIndex)
			if (tree.libIsChecked(groupIndex, libIndex))
			{
				var groupName = tree.groupName(groupIndex);
				var libName = tree.libName(groupIndex, libIndex);
				copyLib(groupName, libName, toDir);
			}
}
//-----------------------------------------------------------------------------------------
function copyLib(groupName, libName, toDir)
{
	// Определение относительно какой папки копировать
	var relDir = '';
	if (groupName == 'plugins' ||
	        groupName == 'qml' ||
	        groupName == 'resources')
		relDir = QTDIR;

	// Копирование библиотеки
	var newFileName = utils.copyFile(libName, toDir, relDir);
	if(!newFileName)
	{
		log.addError('Error: Can`t copy file "' + libName + '" to "' + toDir + '"');
		return;
	}

	// Патчинг Qt5Core, изменение вшитой переменной qt_prfxpath=.
	// http://www.riuson.com/blog/post/qtcore-hard-coded-paths
	if(isQtCoreLib(libName))
	{
		if (!utils.patchFile(newFileName, "qt_prfxpath=", "."))
			log.addError('Can not patch "' + newFileName + '"');
	}

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