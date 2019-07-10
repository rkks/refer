from nose.tools import raises


@raises(TypeError, ValueError)
def test_case01():
    raise TypeError("This test passes")


@raises(Exception)
def test_case02():
    pass
