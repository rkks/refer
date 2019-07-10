from nose.tools import with_setup


def setUpModule():
    """called once, before anything else in this module"""
    print("\nIn setUpModule()...")


def tearDownModule():
    """called once, after everything else in this module"""
    print("\nIn tearDownModule()...")


def setup_function():
    """setup_function(): use it with @with_setup() decorator"""
    print("\nsetup_function()...")


def teardown_function():
    """teardown_function(): use it with @with_setup() decorator"""
    print("\nteardown_function()...")


def test_case01():
    print("In test_case01()...")


def test_case02():
    print("In test_case02()...")


@with_setup(setup_function, teardown_function)
def test_case03():
    print("In test_case03()...")
