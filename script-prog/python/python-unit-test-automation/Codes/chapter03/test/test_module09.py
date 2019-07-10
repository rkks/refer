from mypackage.mymathlib import *
import unittest


math_obj = 0


def setUpModule():
        """called once, before anything else in the module"""
        print("In setUpModule()...")
        global math_obj
        math_obj = mymathlib()


def tearDownModule():
        """called once, after everything else in the module"""
        print("In tearDownModule()...")
        global math_obj
        del math_obj


class TestClass10(unittest.TestCase):

        @classmethod
        def setUpClass(cls):
                """called only once, before any test in the class"""
                print("In setUpClass()...")

        def setUp(self):
                """called once before every test method"""
                print("\nIn setUp()...")

        def test_case01(self):
                print("In test_case01()")
                self.assertEqual(math_obj.add(2, 5), 7)

        def test_case02(self):
                print("In test_case02()")

        def tearDown(self):
                """called once after every test method"""
                print("In tearDown()...")

        @classmethod
        def tearDownClass(cls):
                """called once, after all the tests in the class"""
                print("In tearDownClass()...")


