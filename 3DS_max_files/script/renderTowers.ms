rendSaveFile = TRUE
rendTimeType = 1

outpath = "D:\\AGK\\AGK_libraries\\apps\\Test_app_tower_defence\\Test_app\\src\\main\\assets\\images\\towers\\"

buttonParts = execute("$" + "btn" +"*")
buttonFilter = getNodeByName "btn-filter-inactive"
buttonBorder = getNodeByName "btn-border"

borderColN = color 10 10 255
borderColP = color 10 255 10
borderColI = color 40 40 40

theCam = getNodeByName "theCam"
theCamTarget = getNodeByName "theCamTarget"
viewport.setCamera theCam

camOffset1 = [0, 0, 200]
camOffset2 = [0, 0, 250]

prefixes = #("T", "TA", "TB")
ranges = #(1, 12, 12)
hide $*

for j= 1 to 3 do
(
	prefix = prefixes[j]
	
	for i = 1 to ranges[j] do
	(
		id = formattedPrint i format:"03d"		
		towerName = prefix +  id
		dir = outpath + towerName
		makeDir dir
		break
		towerBaseName = towerName + "-base"
		towerBase = getNodeByName towerBaseName
		towerBasePos = towerBase.pos
		
		buttonParts.pos = towerBasePos
		theCam.pos = towerBasePos + camOffset1
		theCamTarget.pos = towerBasePos
		
		allTowerParts = execute("$" + towerName + "*")
		fixTowerParts = #()
		rotTowerParts = #()
		for towerPart in allTowerParts do 
		(
			if matchPattern towerPart.name pattern:"*rot*" 
				then append rotTowerParts towerPart
				else append fixTowerParts towerPart
		)
		
		hide buttonParts
		
		/*FIX OBJECTS:*/	
		if fixTowerParts.count > 0 do (
			unhide fixTowerParts
			hide rotTowerParts
			outfile = dir + "\\" + towerName + "_f" + ".png"			
			rendOutputFilename =  outfile
			max quick render
		)
		
		/*ROT OBJECTS:*/
		if rotTowerParts.count > 0 do (
			hide fixTowerParts
			unhide rotTowerParts
			outfile = dir + "\\" + towerName + "_r" + ".png"			
			rendOutputFilename =  outfile
			max quick render
		)
		
		/*BUTTONS*/
		theCam.pos = towerBasePos + camOffset2
		unhide allTowerParts
		unhide buttonParts
		
		/*normal button*/
		hide buttonFilter
		buttonBorder.wireColor = borderColN
		outfile = dir + "\\" + towerName + "_btn" + ".png"			
		rendOutputFilename =  outfile
		max quick render
		
		/*pressed button*/
		buttonBorder.wireColor = borderColP
		outfile = dir + "\\" + towerName + "_btn_pressed" + ".png"			
		rendOutputFilename =  outfile
		max quick render
		
		/*inactive button*/
		unhide buttonFilter
		buttonBorder.wireColor = borderColI
		outfile = dir + "\\" + towerName + "_btn_inactive" + ".png"			
		rendOutputFilename =  outfile
		max quick render
		
		hide allTowerParts
	)
)
unhide $*