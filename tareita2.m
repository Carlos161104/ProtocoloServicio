## Copyright (C) 2026 canoj
## Author: canoj <canoj@MSI>
## Created: 2026-03-13

#Condiciones iniciales para la secuencia
a=sigPrimo(20000);
b=sigPrimo(10000);
base=sigPrimo(100000);  #Base debe ser impar (primo)
password =  "sexo";     #contraseña de los dos extremos
x=fix(time);

for i=1:length(password)
  x =(x+uint8(password(i)));  #Pa sumar la contraseña a la llave pública
end

semilla=x; # Guardamos la semilla para que el receptor pueda desencriptar la imagen

X=[];

imagen = imread("gato.jpg");  #Leemos la imagen
tamano = size(imagen);
vector = imagen(:);       #Convertimos la imagen a un vector por columnas


for i=1:length(vector)

  letter = vector(i); # Obtenemos letra por letra de la imagen y


  # Obtener el numero aleatorio y guargarloo en x
  x = mod(a*x+b,base); # El multiplicador debe de ser impar, mientras mas
                      #diferente de la base es mejor

  xNormalizada = fix(x/base*256); #Normalizamos el numero solamente a 256

  X=[X;bitxor(xNormalizada,letter)]; # Al valor pseudoaleatorio, le sumamos la letra en ASCII
end

#X;
#figure;
#plot(X,'.');


clc
Y =[];
for i=1:length(X)
  input = X(i); # Obtenemos el primer numero
  semilla = mod(a*semilla+b,base); # Generamos el primer nuemero aleatorio
  xNormalizada = fix(semilla/base*256);


  character = bitxor(input,xNormalizada);

  Y =[Y;character];


end

reimagen = reshape(Y, tamano);
imshow(uint8(reimagen))

