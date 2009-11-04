try:
	tower_target
except NameError:
	#initialize vars
	tower_lastSpawnTime = { }
	tower_target = { }
	tower_state = { }

	tower_visionRadius = 25
	tower_attackRadius = 15
	tower_health = 500
	tower_damage = 20

	
def tower_onInit(curID):
	setModel(curID,"data/models/tower.md2","data/models/tower.JPG")
	setSpeed(curID, 0, 0)
	setTeam(curID, 1)
	setDamage(curID, tower_damage)
	setHealth(curID, tower_health )


	tower_state[curID] = 0
	tower_target[curID] = 0

	initEntity(curID, 4, 1)

def tower_onThink(curID):

	#hunting
	if tower_state[curID] == 0:
		tower_target[curID] = getNearestEntity(curID, -1, 1, 2)

		if tower_target[curID] != 0:

			if distance(tower_target[curID], curID) <= tower_visionRadius:
				tower_state[curID] = 1

			else:
				tower_target[curID] = getNearestEntity(curID, -1, 2, 2)
				if tower_target[curID] != 0:
					if distance(tower_target[curID], curID) <= tower_visionRadius:
						tower_state[curID] = 2
		else:
			tower_target[curID] = getNearestEntity(curID, -1, 2, 2)
			if tower_target[curID] != 0:
				if distance(tower_target[curID], curID) <= tower_visionRadius:
					tower_state[curID] = 2

	# attacking a unit
	if tower_state[curID] == 1: 
	
		if getHealth(tower_target[curID]) <= 0:
			tower_state[curID] = 0

		if distance(curID, tower_target[curID]) < tower_attackRadius:
			attack(curID, tower_target[curID])
			

	# attacking a building... keep wary for units closing in
	if tower_state[curID] == 2:
		if distance(curID, tower_target[curID]) < tower_attackRadius:
			attack(curID, tower_target[curID])
		
 
def tower_onAttacked(curID, attackerID):

	if tower_state[curID] != 1:
		if distance(curID, attackerID) < tower_visionRadius:
			echo ("tower returning fire!")
			tower_target[curID] = attackerID
			tower_state[curID] = 1
	

	
def tower_onSelected(curID):
	pass
	
def tower_onUnselected(curID):
	pass
