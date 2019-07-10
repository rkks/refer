import unittest


class TestClass08(unittest.TestCase):

	def test_case01(self):
		self.assertTrue("PYTHON".isupper())
		print("\nIn test_case1()")

	def test_case02(self):
		self.assertTrue("Python".isupper())
		print("\nIn test_case2()")

	def test_case03(self):
		self.assertTrue(True)
		print("\nIn test_case3()")

