from nose.tools import assert_equals


def test_case01():
    print("In test_case01()...")
    assert 2+2 == 5


def test_case02():
    print("In test_case02()...")
    assert_equals(2+2, 5)
