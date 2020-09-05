(
	
outpath = "D:\\AGK\\AGK_libs2\\apps\\Test_app_tower_defence\\Test_app\\src\\main\\assets\\images\\towers\\"
minTowerID = 1
maxTowerID = 56
camOffset1 = [0, 0, 200]
camOffset2 = [0, 0, 250]
theCam = getNodeByName "theCam"
theCamTarget = getNodeByName "theCamTarget"
theLight = getNodeByName "omni_light"
viewport.setCamera theCam
	
for j= minTowerID to maxTowerID do
(E
	/*
		Each tower has an ID between minTowerID and maxTowerID: only used so that this script will find them incrementally
		Tower parts are named like: <ID>-<NAME>-<base or gun>-<rot or none>
	
		IDs and names are unique for each towers
	*/
	
	/*get <ID>*/
	id = formattedPrint j format:"03d"	
	print ("* Tower ID    : " + id)
	
	/*get <name>*/
	allTowerParts = execute("$" + id + "*")	
	towerName = (filterString allTowerParts[1].name "-")[2]
	print ("* Tower name: " + towerName)
		
	/*get the tower base (there might be many...) so that we can get its position)*/
	towerBaseName = id + "-" + towerName + "-base"
	print ("* Towerbase: " + towerBaseName)
	towerBase = getNodeByName towerBaseName
	
	towerBasePos = towerBase.pos
	
	/*collect parts that are rotatable (has "rot" in name) and not rotatebale*/
	fixTowerParts = #()
	rotTowerParts = #()
	for towerPart in allTowerParts do (
		if matchPattern towerPart.name pattern:"*rot*" 
			then append rotTowerParts towerPart
			else append fixTowerParts towerPart					
	)

	print "\t + fixed parts:"
	for towerPart in fixTowerParts do (print ("\t\t" + towerPart.name))
	
	print "\t + rotatable parts:"
	for towerPart in rotTowerParts do (print ("\t\t" + towerPart.name))
	
	/*Render path:*/
	dir = outpath + towerName		
	print ("\t + render dir:" + dir)
		
	theCam.pos = towerBasePos + camOffset1
	theCamTarget.pos = towerBasePos
	theLight.pos = theCam.pos
		
	makeDir dir
		
		
	/*RENDER fixed parts:*/	
	if fixTowerParts.count > 0 do (
		unhide fixTowerParts
		hide rotTowerParts
		outfile = dir + "\\" + towerName + "_f" + ".png"			
		rendOutputFilename =  outfile
		max quick render
	)
	
	/*RENDER rotatable parts:*/
	if rotTowerParts.count > 0 do (
		hide fixTowerParts
		unhide rotTowerParts
		outfile = dir + "\\" + towerName + "_r" + ".png"			
		rendOutputFilename =  outfile
		max quick render
	)
	
	/*all parts without button background (new 2020-05-02)*/ 
	unhide fixTowerParts
	unhide rotTowerParts
	outfile = dir + "\\" + towerName + "_icon" + ".png"			
	rendOutputFilename =  outfile
	max quick render
	
	print "\n"
)


)