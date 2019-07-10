from mypackage.mymathlib import *


class TestClass01:
	def test_case01(self):
        	print("In test_case01()")
        	assert mymathlib().add(2, 5) == 7
