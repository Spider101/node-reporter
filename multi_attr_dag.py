from random import randint
from copy import deepcopy
import networkx as nx
import pdb

numbers = []
diff_table = []
set_metadata = []
playerPool = {"batsman" : [], "bowler" : [], "keeper" : [], "allRounder" : []}
playerTypes= ["batsman", "bowler", "keeper", "allRounder"]
budget = 0
average = 0

class Node(object):
	
	def __init__(self):
		self._shifts = []

	@property
	def shifts(self):
	    return self._shifts
	
	@shifts.setter
	def shifts(self, shifts):
		self._shifts = shifts

class DAG(object):

	graph = nx.DiGraph()
	queue = []
	explored_nodes = []

	''' Each block is represented by a list of shifts corresponding to each bit in the block'''
	def initialize_dag(self):
		node = Node()
		for i in xrange(0, len(set_metadata)):
			block = []
			for j in xrange(0, set_metadata[i]["subset_strength"]):
				block.append(0)
			node.shifts.append(block)
		self.queue.append(node)
		root = "".join(str(w) for w in flatten(node.shifts))
		if set_metadata[0]["subset_strength"] > set_metadata[1]["subset_strength"]:
			root = root + "'"
		#root = summate(node.shifts)
		self.graph.add_node(root, node = { "shifts" : flatten(node.shifts), "value" : summate(node.shifts), "underBudget" : isUnderBudget(summate(node.shifts))})
		print "Initialized config 1"
		print "\nGraph has ", self.graph.number_of_edges(), " edges and ", self.graph.number_of_nodes(), " nodes"

		#print self.graph.nodes(data = True)
		
	def build_dag(self):
		noOfNodes = 0
		while self.queue:
			#gets the first node in queue
			parentNode = self.queue.pop(0)
			parent = "".join(str(w) for w in flatten(parentNode.shifts))
			if set_metadata[0]["subset_strength"] > set_metadata[1]["subset_strength"]:
				parent = parent + "'"
			noOfNodes += 1
			#print "\nNode: ", parentNode.shifts
			#pdb.set_trace()
			if parentNode not in self.explored_nodes:
				for i in xrange(0, len(parentNode.shifts)):
					#limit is (n-r) for each block or set
					limit = set_metadata[i]["limit"]
					block = parentNode.shifts[i]
					blockBreak = 0
					#check if last occurrence of zero is at the end of the block, ie unbroken block
					if 0 in block and block[::-1].index(0) == 0:
						#print "We got an unbroken block"
						childNode = Node()
						childNode.shifts = deepcopy(parentNode.shifts)
						childNode.shifts[i][len(block) - 1] += 1
						self.queue.append(childNode)
						successor = "".join(str(w) for w in flatten(childNode.shifts))
						if set_metadata[0]["subset_strength"] > set_metadata[1]["subset_strength"]:
							successor = successor + "'"
						self.graph.add_node(successor, node = { "shifts" : flatten(childNode.shifts), "value" : summate(childNode.shifts), "underBudget" : isUnderBudget(summate(childNode.shifts))})
						self.graph.add_edge(successor, parent)
						'''print "\nParent Node: ", parentNode.shifts, " No of nodes so far", noOfNodes, "and added ", (self.graph.number_of_nodes() - noOfNodes), " new nodes"
						for node, attr in self.graph.nodes_iter( data = True ):
							print node, attr'''
					
					#broken block
					else:
						#check if first bit is unshifted, ie a 1
						if block[0] == 0:
							#find posn of bit after the last occurrence of 0 in the block; marks the start of second block
							blockBreak = len(block) - block[::-1].index(0)

							#check if its legal to increase shift posn of last bit of unbroken sub-block
							if block[blockBreak - 1] < limit:
								#rule 2(a)
								childNode = Node()
								childNode.shifts = deepcopy(parentNode.shifts)
								childNode.shifts[i][blockBreak - 1] += 1
								self.queue.append(childNode)	
								successor = "".join(str(w) for w in flatten(childNode.shifts))				
								if set_metadata[0]["subset_strength"] > set_metadata[1]["subset_strength"]:
									successor = successor + "'"
								self.graph.add_node(successor, node = { "shifts" : flatten(childNode.shifts), "value" : summate(childNode.shifts), "underBudget" : isUnderBudget(summate(childNode.shifts))})
								self.graph.add_edge(successor, parent)
								'''print "\nParent Node: ", parentNode.shifts, " No of nodes so far", noOfNodes, "and added ", (self.graph.number_of_nodes() - noOfNodes), " new nodes"
								for node, attr in self.graph.nodes_iter( data = True ):
									print node, attr'''
								#print "Child node: ", childNode.shifts
						
						#check if its legal to increase the shift posn of blockbreak bit
						if block[blockBreak] < limit:
							if blockBreak == len(block) - 1 or blockBreak < len(block) - 1 and block[blockBreak + 1] > block[blockBreak]:
								childNode = Node()
								childNode.shifts = deepcopy(parentNode.shifts)
								childNode.shifts[i][blockBreak] += 1
								self.queue.append(childNode)
								successor = "".join(str(w) for w in flatten(childNode.shifts))
								if set_metadata[0]["subset_strength"] > set_metadata[1]["subset_strength"]:
									successor = successor + "'"
								self.graph.add_node(successor, node = { "shifts" : flatten(childNode.shifts), "value" : summate(childNode.shifts), "underBudget" : isUnderBudget(summate(childNode.shifts))})
								self.graph.add_edge(successor, parent)
								'''print "\nParent Node: ", parentNode.shifts, " No of nodes so far", noOfNodes, "and added ", (self.graph.number_of_nodes() - noOfNodes), " new nodes"
								for node, attr in self.graph.nodes_iter( data = True ):
									print node, attr'''
								#print "Child node: ", childNode.shifts

						break

				self.explored_nodes.append(parentNode)
			else:
				continue

		print "No of nodes in Dag by my count: ", noOfNodes
		print "\nGraph has ", self.graph.number_of_edges(), " edges and ", self.graph.number_of_nodes(), " nodes"
		'''for node, attr in self.graph.nodes_iter( data = True ):
			print node, attr'''


	def draw_dag(self):
		print "\nGraph has ", self.graph.number_of_edges(), " edges and ", self.graph.number_of_nodes(), " nodes"
		
		nx.write_gml(self.graph, 'paperDag.gml')



def isUnderBudget(value):
	#print "Budget is: ", budget, " and value is ", int(value), "\n"
	if int(value) <= budget:
		return "True"
	else: 
		return "False"

def flatten(blocks):
	ls = []
	for i in xrange(0, len(blocks)):
		for j in xrange(0,len(blocks[i])):
			ls.append(blocks[i][j])
	return ls

def summate(blocks):
	sum = 0
	try:
		for i in xrange(0,len(blocks)):
			for j in xrange(0, len(blocks[i])):
				sum += numbers[i][j + blocks[i][j]]
		return str(sum)
	except IndexError:
		print "Gotcha: ", i, j, blocks, numbers
		return 0

def gen_diff_list(values):
	diff_list = []
	for i in xrange(0, len(values) - 1):
		diff_list.append(values[i+1] - values[i])
	return diff_list

def populatePlayerPool():
	file_b = open("batsman.txt", "r")
	file_bl = open("bowler.txt", "r")
	file_w = open("wicketkeeper.txt", "r")
	file_ar = open("allRounder.txt", "r")
	for line in file_b:
	   	word = line.split()[0]
	   	playerPool["batsman"].append(int(word))
	#print playerPool["batsman"]
	for line in file_ar:
	   	word = line.split()[0]
	   	playerPool["allRounder"].append(int(word))
	for line in file_bl:
	   	word = line.split()[0]
	   	playerPool["bowler"].append(int(word))
	for line in file_w:
	   	word = line.split()[0]
	   	playerPool["keeper"].append(int(word))
	'''print "The Player Pool: \n"
	for playerType in playerPool:
	 	print playerType, " : \n", playerPool[playerType], "\n"'''

def run():
	#first dag
	global set_metadata
	no_of_attrs = (int)(raw_input("Enter the number of attributes: "))
	for i in xrange(0, no_of_attrs):
		values = []
		noOfPlayers = len(playerPool[playerTypes[i]])
		n, r = (raw_input("Enter number of elements with attribute " + str(i+1) + " and number of elements required from set " + str(i+1) + ": ")).split()
		set_metadata.append({ "elem_strength" : int(n), "subset_strength" : int(r) , "limit" : int(n) - int(r)})
		for j in xrange(0, set_metadata[i]["elem_strength"]):
			values.append( playerPool[playerTypes[i]][randint(0,noOfPlayers-1)] )
		values.sort()
		numbers.append(values)
		diff_table.append(gen_diff_list(values))
	global budget
	budget = (int)(raw_input("Enter budget for team: "))
	print "Values : ", numbers#, "Difference Table:", diff_table
	graph = DAG()
	graph.initialize_dag()
	graph.build_dag()

	#second dag
	set_metadata = []
	no_of_attrs = (int)(raw_input("Enter the number of attributes: "))
	for i in xrange(0, no_of_attrs):
		values = []
		noOfPlayers = len(playerPool[playerTypes[i]])
		n, r = (raw_input("Enter number of elements with attribute " + str(i+1) + " and number of elements required from set " + str(i+1) + ": ")).split()
		set_metadata.append({ "elem_strength" : int(n), "subset_strength" : int(r) , "limit" : int(n) - int(r)})
	#global budget
	budget = (int)(raw_input("Enter budget for team: "))
	print "Values : ", numbers#, "Difference Table:", diff_table
	#pdb.set_trace()
	graph.initialize_dag()
	graph.build_dag()

	#draw the combined dag
	graph.draw_dag()

populatePlayerPool()
run()
