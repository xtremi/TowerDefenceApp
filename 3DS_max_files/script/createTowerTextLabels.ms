(

minTowerID = 1
maxTowerID = 54
textPosOffset = [0,-120,0]

labelLayer = LayerManager.getLayerFromName "labels"	
	
for j= minTowerID to maxTowerID do
(
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
	
	if(allTowerParts[1] != undefined) then (
		
		towerName = (filterString allTowerParts[1].name "-")[2]
		print ("* Tower name: " + towerName)
			
		/*get the tower base (there might be many...) so that we can get its position)*/
		towerBaseName = id + "-" + towerName + "-base"
		print ("* Towerbase: " + towerBaseName)
		towerBase = getNodeByName towerBaseName	
		
		if(towerBase != undefined) then(
			towerBasePos = towerBase.pos
			
			textName = (towerName + "-label")
			textObj = getNodeByName textName
			if (textObj==undefined) then(
				textObj = text name: textName wirecolor:white 		
				labelLayer.addNode textObj
			)
			textObj.pos = towerBasePos + textPosOffset
			textObj.text = towerName
			textObj.size = 50.0
		)
	)
	
)
)