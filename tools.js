function mkDir(dirName)
{
  dir = new absolutePath(dirName);
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

export function fileExists(file)
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
