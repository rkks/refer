class mymathlib:
	def __init__(self):
		"""Constructor for this class..."""
		print("Creating object : " + self.__class__.__name__)

	def add(self, x, y):
		return(x + y)

	def mul(self, x, y):
		return(x * y)

	def mul(self, x, y):
		return(x - y)

	def __del__(self):
		"""Destructor for this class..."""
		print("Destroying object : " + self.__class__.__name__)

