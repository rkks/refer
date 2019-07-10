from nose.tools import timed
import time


@timed(.1)
def test_case01():
    time.sleep(.2)

