# Definición de una función
def saludar(nombre):
    """Esta función imprime un saludo."""#Es un comentario tipo docstring, que explica la funcion y sirve para el parametro help()
    print("\nSaludo de la Función:")
    print("Hola,", nombre)

# Llamada a la función
saludar("Juan")
saludar("Ana")

# Función con retorno de valor
def sumar(a, b):
    """Esta función devuelve la suma de dos números."""
    resultado = a + b
    return resultado

# Llamada a la función con retorno de valor
resultado_suma = sumar(3, 5)
print("\nFunción con Retorno de Valor:")
print("La suma es:", resultado_suma)

# Función con parámetros opcionales y predeterminados
def saludar_persona(nombre, saludo="Hola"):
    """Esta función saluda a una persona con un saludo personalizado."""
    print("\nFunción con Parámetros Opcionales y Predeterminados:")
    print(saludo + ",", nombre)

# Llamada a la función con parámetros opcionales y predeterminados
saludar_persona("Juan")
saludar_persona("Ana", "¡Hola!")
