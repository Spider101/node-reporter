import matplotlib.pyplot as plt
import networkx as nx
from random import randint
import pdb

class Node(object):

	def __init__(self):
		self._shifts = []
		self._lookup = []

	@property
	def shifts(self):
	    return self._shifts
	
	@shifts.setter
	def shifts(self, shifts):
		self._shifts = shifts

class DAG(object):
	nascent_nodes = []
	explored_nodes = []
	graph = nx.DiGraph()

	def initialize_dag(self,r):
		node = Node()
		for x in xrange(0,r):
			node.shifts.append(0)
		self.nascent_nodes.append(node)
		root = "".join(str(w) for w in node.shifts)
		self.graph.add_node(root, node = { "shifts" : node.shifts, "lookup" : [] })
		print "Setting child node: ", self.graph.node[root], "\n"

	def build_nodes_list(self, n, r):
		rule1 = False
		rule2 = False
		#pdb.set_trace()
		while self.nascent_nodes:
			node = self.nascent_nodes.pop(0)
			parent = "".join(str(w) for w in node.shifts)
			children = 0
			for i in xrange(r-1, -1, -1):
				rule1 = node.shifts[i]  < (n-r) and i==r-1
				rule2 = i+1 < r and node.shifts[i+1] - node.shifts[i] == 1
				if rule1 or rule2:
					child = Node()
					child.shifts = node.shifts[:]
					child.shifts[i] += 1
					self.nascent_nodes.append(child)
					
					#create the path and configs for the the child nodes
					successor = "".join(str(w) for w in child.shifts)
					self.graph.add_node(successor, node = { "shifts" : child.shifts, "lookup" : [] })
					if rule2:
						break
				elif i+1 < r and node.shifts[i+1] - node.shifts[i] > 0:
					break
			self.explored_nodes.append(node)

	def build_Dag(self, n ,r):
		child, parent = [], []
		for node, attr in self.graph.nodes_iter( data = True ):
			#pdb.set_trace()
			parent = attr["node"]["shifts"]
			lookup = []
			for i in xrange(0, len(parent)):
				child = parent[:]
				if i < len(parent) - 1 and parent[i+1] - parent[i] > 0 or i == len(parent) - 1 and parent[i] < n-r:
					child[i] += 1
					print "Connecting ", child, " to ", parent

					lookup.append({ "diff" : parent[i] + i, "shift" : i })
					#print "Edge from ", self.graph.get_node(), " to ", node
					successor = "".join(str(w) for w in child)
					self.graph.add_edge(node, successor) 
			
			ancestor = "".join(str(w) for w in parent)
			self.graph.node[ancestor]["node"]["lookup"] = lookup;
			print "Updating parent node: ", self.graph.node[ancestor];

	def relax_Dag(self, n, r, diff_list):
		child, max_shift = [], {}
		for node, attr in self.graph.nodes_iter( data = True ):
			child = attr["node"]["shifts"][:]
			lookups = attr["node"]["lookup"]
			if len(lookups) > 1:
				min_shift = lookups[0]
				for i in xrange(1, len(lookups)):
					#print diff_list[lookups[i]["diff"]], " & ", diff_list[min_shift["diff"]]
					if diff_list[lookups[i]["diff"]] < diff_list[min_shift["diff"]]:
						child[min_shift["shift"]] += 1	
						min_shift = lookups[i]
					else:
						child[lookups[i]["shift"]] += 1
					successor = "".join(str(w) for w in child)
					self.graph.remove_edge(node, successor) 


	def show_Dag(self):
		for node in self.explored_nodes:
			print node.shifts

	def draw_Dag(self):
		print "Graph has ", self.graph.number_of_edges(), " edges and ", self.graph.number_of_nodes(), " nodes"
		nx.write_gml(self.graph, 'dag#0.gml')

def populate_list(length):
	ls = []
	while(length):
		ls.append(randint(10,99))
		length = length - 1
	return ls

def build_diff_matr(matr, jump):
	ls = []
	for x in xrange(0,len(matr)-jump):
		ls.append(matr[x] - matr[x+jump])
	return ls

def build_diff_dict(numbers):
	no_of_attrs = len(numbers)
	diff_dict = {}
	for x in xrange(1,no_of_attrs):
		diff_dict[x] = build_diff_matr(numbers, x)
	print "The dictionary of difference matrices: "
	print diff_dict
	return diff_dict


def run():
	numbers = populate_list(10)
	numbers.sort(reverse = True)
	print "The list of numbers are: "
	print numbers
	diff_dict = build_diff_dict(numbers)
	#n = (int)(raw_input("Enter number of integers(n): "))
	#r = (int)(raw_input("Enter size of subset(r): "))
	graph = DAG()
	graph.initialize_dag(3)
	graph.build_nodes_list(6, 3)
	#graph.show_Dag()
	graph.build_Dag(6,3)
	'''for key in diff_dict:
		print "Using difference table ", key, ": ", diff_dict[key]
		graph.relax_Dag(6,3,diff_dict[key])
		break'''
	#graph.draw_Dag()

run()