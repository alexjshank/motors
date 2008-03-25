curTeam = 0

echo ('ria: '+str(ria)+', rib: '+str(rib)+', ric: '+str(ric)+', rid: '+str(rid))

if ria == 0 : 
	ria = getNearestEntity(curID, 4, 0, curTeam)

if ric == 2 and rib != 0:
	setPosition(ric,getPosition(curID))
	
if getState(curID) == 2:
	if ric == 0:
		if getTime() - rid > 30:
			ric = 1
			echo('unit('+str(curID)+') searching for food!')
	if ric == 1:
		if rib == 0:
			rib = getNearestEntity(curID, 5, 0, curTeam)
			
		if rib != 0:
			tempD = distance(curID,rib)
			echo ('distance to target: '+str(tempD))
			
			if tempD < 3:
				killent(rib)
				ric = 2
				echo('unit('+str(curID)+') gathering food')
			else:
				pathToEnt(curID, rib)
				echo('unit('+str(curID)+') walking to food')

	if ric == 2:
		if ria != 0:
			if distance(curID,ria) < 5:
				rid = getTime()
#				UptakeResources(ria, rib)
				rib = 0
				ric = 0
				echo('unit('+str(curID)+') eating! yummmm')
				
