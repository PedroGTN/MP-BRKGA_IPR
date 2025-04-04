import os
from multiprocessing import Pool

def executa(comando):
    print("Executarei: " + str(comando))
    os.system(str(comando))


comandos = [
    'echo 1',
    'echo 2',
    'echo 3',
    'echo 4',
    'echo 5',
    'echo 6'
]

pool = Pool(processes=3)
pool.map(executa, comandos)