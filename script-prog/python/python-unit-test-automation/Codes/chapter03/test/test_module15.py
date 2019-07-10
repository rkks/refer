import unittest


def setUpModule():
#   raise Exception
    pass


def tearDownModule():
#   raise Exception
    pass


class TestClass15(unittest.TestCase):

    @classmethod
    def setUpClass(cls):
#       raise Exception
        pass

    def setUp(self):
#       raise Exception
        pass

    def test_case01(self):
        self.id()

    def tearDown(self):
#       raise Exception
        pass

    @classmethod
    def tearDownClass(cls):
#       raise Exception
        pass

