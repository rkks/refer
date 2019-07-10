import unittest


class Calculator:

	def add1(self, x, y):
		return x + y

	def add2(self, x, y):
		number_types = (int, float, complex)
		if isinstance(x, number_types) and isinstance(y, number_types):
			return x + y
		else:
			raise ValueError


calc = 0


class TestClass16(unittest.TestCase):

	@classmethod
	def setUpClass(cls):
		global calc
		calc = Calculator()

	def setUp(self):
		print("\nIn setUp()...")

	def test_case01(self):
		self.assertEqual(calc.add1(2, 2), 4)
	
	def test_case02(self):
		self.assertEqual(calc.add2(2, 2), 4)

	def test_case03(self):
		self.assertRaises(ValueError, calc.add1, 2, 'two')
	
	def test_case04(self):
		self.assertRaises(ValueError, calc.add2, 2, 'two')
		
	def tearDown(self):
		print("\nIn tearDown()...")
    
	@classmethod
	def tearDownClass(cls):
		global calc
		del calc
