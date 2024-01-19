# Creación de una lista
mi_lista = [1, 2, 3, 4, 5]
print("Lista:\n",mi_lista)

# Ciclo for
print("\nElementos con un FOR")

for elemento in mi_lista:
    print(elemento)

# Ciclo while
print("\nElementos con un While")

indice = 2 #*Variable inicial
while indice < len(mi_lista):
    print(mi_lista[indice])
    indice += 1
