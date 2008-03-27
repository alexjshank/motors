

def soldier_onInit(curID):
	
	
def soldier_onThink(curID):
	if getHealth(curID) < 25:
		if soldier_enemy[curID] != 0 and distance(curID, soldier_enemy[curID]) > 10:
			soldier_state[curID] = 999
		
	if soldier_state[curID] == 0:
		if getTeam(curID) == 1: otherTeam = 2
		else: otherTeam = 1
		soldier_enemy[curID] = getNearestEntity(curID, -1, 0, otherTeam)
		if distance(curID, soldier_enemy[curID]) > 15:
			pathToEnt(soldier_enemy[curID])
		else:
			soldier_state[curID] = 1
	else if soldier_state[curID] == 1:
		if distance(curID, soldier_enemy[curID]) > 15:
			pathToEnt(soldier_enemy[curID])
		else:
			attack(soldier_enemy[curID])
			
			if getHealth(soldier_enemy[curID]) <= 0:
				soldier_state[curID] = 0
				