# Creación de una lista
mi_lista = [1, 2, 3, 4, 5]
print("Lista:\n",mi_lista)

# Agregar elementos al final de la lista
mi_lista.append(6)
print("Lista con otro elemento al final:\n",mi_lista)

# Insertar elemento en una posición específica
mi_lista.insert(2, 7)
print("Lista agregando un elemento en la posicion 2:\n",mi_lista)

# Eliminar elemento por valor
mi_lista.remove(4)#!Solo borra el primer elemento con ese valor
print("Lista borrando el elemento de valor 4:\n",mi_lista)

# Eliminar elemento por índice
elemento_eliminado = mi_lista.pop(1)
print("Lista borrando el elemento en la posicion 1:\n",mi_lista)
