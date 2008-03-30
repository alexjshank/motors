try : 
	woman_state
except NameError :
	woman_state = { }
	woman_target = { }
	woman_mill = { }
	woman_time = { }

def woman_onInit(curID):
	woman_state[curID] = 0
	woman_target[curID] = 0
	woman_mill[curID] = 0
	woman_time[curID] = 0

	setModel(curID,"data/models/villagewoman.md2","data/models/VillagerWoman.JPG")
	setSpeed(curID,3,1)

def woman_onThink(curID):

	curTeam = getTeam(curID)
	if woman_state[curID] == 0 : 
		woman_mill[curID] = getNearestEntity(curID, 4, 0, curTeam)

	if woman_state[curID] == 2 and woman_target[curID] != 0:
		setPosition(woman_target[curID],curID)
		
	if getState(curID) == 2:
		if woman_state[curID] == 0:
			if getTime() - woman_time[curID] > 10:
				woman_state[curID] = 1
				
		if woman_state[curID] == 1:
			if woman_target[curID] == 0:
				woman_target[curID] = getNearestEntity(curID, 5, 0, curTeam)
				
			if woman_target[curID] != 0:
				tempD = distance(curID,woman_target[curID])
				
				if tempD < 3:
					killent(woman_target[curID])
					woman_state[curID] = 2
				else:
					pathToEnt(curID, woman_target[curID])

		if woman_state[curID] == 2:
			if woman_mill[curID] != 0:
				if distance(curID,woman_mill[curID]) < 5:
	#				UptakeResources(woman_mill[curID], woman_target[curID])
					woman_target[curID] = 0
					woman_state[curID] = 0
					woman_time[curID] = getTime()
					
				else:
					pathToEnt(curID,woman_mill[curID])
 
def woman_onAttacked(curID, attackerID):
	echo("onattacked()")
	
def woman_onDeath(curID):
	echo("death")
	
def woman_onSelected(curID):
	echo("selected")
	
def woman_onUnselected(curID):
	echo("unselected")
