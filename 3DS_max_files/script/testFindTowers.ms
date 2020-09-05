(
	
minTowerID = 1
maxTowerID = 48


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
	towerName = (filterString allTowerParts[1].name "-")[2]
	print ("* Tower name: " + towerName)
		
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
	
	
	print "\n"
)


)