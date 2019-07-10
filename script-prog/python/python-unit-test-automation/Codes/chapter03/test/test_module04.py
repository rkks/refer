import unittest
import inspect


class TestClass04(unittest.TestCase):

        def test_case01(self):
                print("\nClassname : " + self.__class__.__name__)
                print("Running Test Method : " + inspect.stack()[0][3])


class TestClass05(unittest.TestCase):

        def test_case01(self):
                print("\nClassname : " + self.__class__.__name__)
                print("Running Test Method : " + inspect.stack()[0][3])

if __name__ == '__main__':
        unittest.main(verbosity=2)

