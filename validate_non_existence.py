F = lambda a,b,c: lambda n: a*n*n+b*n+c
G = lambda alpha, beta, gamma, delta: lambda X,Y: Y+alpha*X+beta*X*Y+gamma*X*X*Y+delta*X*X*Y*Y
from itertools import product
class Z4:
    def __init__(self, n):
        self.value = int(n) % 4
    def __add__(self, other):
        return Z4(self.value+other.value)
    def __str__(self):
        return str(self.value)
    def __neg__(self):
        return Z4(4-self.value)
    def __mul__(self, other):
        return Z4(self.value*other.value)
    def __eq__(self, other):
        if(type(other) is int):
            return self == Z4(other)
        return self.value == other.value
    @staticmethod
    def __iter__():
        return iter([Z4(0), Z4(1), Z4(2), Z4(3)])
def isTrivial(g):
    for n in Z4.__iter__():
        if g(n) == 0 or g(n) == 3:
            return False
    return True
def isValid(f,a,b,c):
    for x,k in product(Z4.__iter__(), repeat=2):
        if k*b==x*x+f(a,c):
            print(f'{k}*{b}={x}^2+f({a},{c})')
            return False
    return True
for (alpha, beta, gamma, delta) in product(Z4.__iter__(), repeat=4):
    print(f'f(X,Y)=Y+{alpha}X+{beta}XY+{gamma}X²Y+{delta}X²Y²')
    isPossible = True
    for (a,b,c) in product(Z4.__iter__(), repeat = 3):
        if(isTrivial(F(a,b,c))):
            if not isValid(G(alpha, beta, gamma, delta), a, b, c):
                isPossible = False
                break
    if(isPossible):
        print("isPossible")
