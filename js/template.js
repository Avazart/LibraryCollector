

function init()
{
	log.addInfo("init()","green");

	tree.clearGroups();
	// ...

}


function update(libs)
{
	log.addInfo("update()","blue");

	tree.clearLibs();
	for(let i in libs)
	{
		log.addInfo(libs[i]);
	}
}

function copy(toDir)
{
	log.addInfo("copy()","magenta");

	for(let groupIndex=0; groupIndex<tree.groupCount(); ++groupIndex)
		for(let libIndex=0; libIndex<tree.libCount(groupIndex); ++libIndex)
			if(tree.libIsChecked(groupIndex,libIndex))
			{
				log.addInfo(groupName+" "+libName+" "+toDir);
			}
}