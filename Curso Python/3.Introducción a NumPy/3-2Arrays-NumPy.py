import numpy as np

# Creación de un array unidimensional
array_1d = np.array([1, 2, 3, 4, 5])
print("Matris 1d\n",array_1d)

# Creación de un array bidimensional (matriz)
array_2d = np.array([[1, 2, 3], [4, 5, 6], [7, 8, 9]])
print("Matris 2d\n",array_2d)


# Dimensiones de un array
dimensiones = array_2d.shape
print("Dimesion M2\n",dimensiones)

# Tipo de datos en un array
tipo_de_datos = array_2d.dtype
print("Tipo de datos M2\n",tipo_de_datos)

# Acceder a un elemento en un array
elemento = array_1d[2]
print("Elemento posicion 2  M1\n",elemento)

# Slicing en arrays
subarray = array_1d[1:4]
print("Elemento posicion 1 a 4  M1\n",subarray)

# Operaciones aritméticas
suma = array_1d + array_1d
print("Resultado de la suma M1\n",suma)
multiplicacion = array_1d * 3
print("Resultado de la multiplicacion M1\n",multiplicacion)

# Producto de matrices
resultado_producto = np.dot(array_2d, array_2d)
print("Resultado de la producto M2\n",resultado_producto)

# !Multiplicacion, sirve para multiplicar un valor por valor del elemento de la 
# !matris pero el producto, da el producto verctorial


# Crea un programa que define dos arrays NumPy unidimensionales, 
# realiza una operación aritmética entre ellos y muestra el resultado.


# Creación de dos arrays unidimensionales
array_a = np.array([1, 2, 3])
array_b = np.array([4, 5, 6])

# Operación aritmética
resultado = array_a + array_b

# Mostrar resultado
print("Array A:", array_a)
print("Array B:", array_b)
print("Resultado de la suma:", resultado)


