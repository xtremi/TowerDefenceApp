
(
	newIndex = 56
	newIndexStr = formattedPrint newIndex format:"03d"	
	print newIndexStr
	
	for item in selection do(
		currName = item.name		
		curIndexStr = (filterString currName "-")[1]
		
		
		newName = substituteString currName curIndexStr newIndexStr
		
		print currName
		print newName
		item.name = newName
		
	)
)