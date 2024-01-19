import numpy as np

# Creación de dos arrays NumPy
array_a = np.array([1, 2, 3])
array_b = np.array([4, 5, 6])

# Operaciones matemáticas
suma = array_a + array_b
resta = array_a - array_b
multiplicacion = array_a * array_b
division = array_a / array_b
potencia = array_a ** 2

# Funciones matemáticas
seno_array = np.sin(array_a)
coseno_array = np.cos(array_a)
logaritmo_array = np.log(array_a)

# Manipulación de datos
array_2d = np.array([[1, 2, 3], [4, 5, 6]])
transpuesta = np.transpose(array_2d)

array_reshaped = np.reshape(array_a, (3, 1))

# Mostrar resultados
print("Variables:")
print(array_a)
print(array_a)
print(array_2d)
print("\nOperaciones matemáticas:")
print("Suma:", suma)
print("Resta:", resta)
print("Multiplicación:", multiplicacion)
print("División:", division)
print("Potencia:", potencia)

print("\nFunciones matemáticas:")
print("Seno:", seno_array)
print("Coseno:", coseno_array)
print("Logaritmo:", logaritmo_array)
print("\nManipulación de datos:")
print("Matriz Transpuesta:")
print(transpuesta)
print("Array Reshape:")
print(array_reshaped)
