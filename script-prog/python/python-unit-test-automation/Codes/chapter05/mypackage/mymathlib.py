class mymathlib:
    def __init__(self):
        """Constructor for the class"""
        print("Creating object for the class : " + self.__class__.__name__)

    def add(self, x, y):
        return(x + y)

    def mul(self, x, y):
        return(x * y)

    def mul(self, x, y):
        return(x - y)

    def __del__(self):
        """Destructor for the class"""
        print("Destroying object for the class : " + self.__class__.__name__)
