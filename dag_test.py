from random import randint
import pdb

class Node(object):
	shifts = []
	lookup = []
	level = 0
	config = 0

class DAG(object):
	nascent_nodes = []
	explored_nodes = []
	path_matr = []

	def initialize_dag(self,r):
		node = Node()
		for x in xrange(0,r):
			node.shifts.append(0)
		self.nascent_nodes.append(node)

	def build_nodes_list(self, n, r):
		rule1 = False
		rule2 = False
		#pdb.set_trace()
		while self.nascent_nodes:
			node = self.nascent_nodes.pop()
			children = 0
			for i in xrange(r-1, -1, -1):
				rule1 = node.shifts[i] + 1 <= (n-r) and i==r-1
				rule2 = i+1 < r and node.shifts[i+1] - node.shifts[i] == 1
				if rule1 or rule2:
					child = Node()
					child.shifts = node.shifts[:]
					child.shifts[i] += 1
					child.level = node.level + 1
					self.nascent_nodes.append(child)
					children += 1
					node.lookup.append(node.shifts[i] + i)
					#TODO: create the path and configs for the the child nodes
					
					if rule2:
						break
				elif i+1 < r and node.shifts[i+1] - node.shifts[i] > 0:
					break
			self.explored_nodes.append(node)

	def show_Dag(self):
		for node in self.explored_nodes:
			print node.shifts



def populate_list(length):
	ls = []
	while(length):
		ls.append(randint(10,99))
		length = length - 1
	return ls

def build_diff_matr(matr, jump):
	ls = []
	for x in xrange(0,len(matr)-jump):
		ls.append(matr[x+jump] - matr[x])
	return ls

def build_diff_dict(numbers):
	no_of_attrs = len(numbers)
	diff_dict = {}
	for x in xrange(1,no_of_attrs):
		diff_dict[x] = build_diff_matr(numbers, x)
	print "The dictionary of difference matrices: "
	print diff_dict

def print_lists(lst):
	for item in lst:
		print item.shifts

def run():
	numbers = populate_list(10)
	numbers.sort()
	print "The list of numbers are: "
	print numbers
	n = (int)(raw_input("Enter number of integers(n): "))
	r = (int)(raw_input("Enter size of subset(r): "))
	graph = DAG()
	graph.initialize_dag(r)
	graph.build_nodes_list(n, r)
	graph.show_Dag()

run()