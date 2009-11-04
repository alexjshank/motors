try : 
	soldier_state
except NameError :
	soldier_state = { }
	soldier_target = { }
	soldier_mill = { }
	soldier_time = { }

def soldier_onInit(curID):
	soldier_state[curID] = 0
	soldier_target[curID] = 0
	soldier_mill[curID] = 0
	soldier_time[curID] = 0

	setModel(curID,"data/models/villagesoldier.md2","data/models/Villagersoldier.BMP")

	initEntity(curID, 5, 1)

def soldier_onThink(curID):

	curTeam = getTeam(curID)
	if soldier_state[curID] == 0 : 
		soldier_mill[curID] = getNearestEntity(curID, 4, 0, curTeam)

	if soldier_state[curID] == 2 and soldier_target[curID] != 0:
		setPosition(soldier_target[curID],curID)
		
	if getState(curID) == 2:
		if soldier_state[curID] == 0:
			if getTime() - soldier_time[curID] > 10:
				soldier_state[curID] = 1
				
		if soldier_state[curID] == 1:
			if soldier_target[curID] == 0:
				soldier_target[curID] = getNearestEntity(curID, 5, 0, curTeam)
				
			if soldier_target[curID] != 0:
				tempD = distance(curID,soldier_target[curID])
				
				if tempD < 3:
					killent(soldier_target[curID])
					soldier_state[curID] = 2
				else:
					pathToEnt(curID, soldier_target[curID])

		if soldier_state[curID] == 2:
			if soldier_mill[curID] != 0:
				if distance(curID,soldier_mill[curID]) < 5:
	#				UptakeResources(soldier_mill[curID], soldier_target[curID])
					soldier_target[curID] = 0
					soldier_state[curID] = 0
					soldier_time[curID] = getTime()
					
				else:
					pathToEnt(curID,soldier_mill[curID])
 
def soldier_onAttacked(curID, attackerID):
	echo("onattacked()")
	
def soldier_onDeath(curID):
	echo("death")
	
def soldier_onSelected(curID):
	echo("selected")
	
def soldier_onUnselected(curID):
	echo("unselected")
