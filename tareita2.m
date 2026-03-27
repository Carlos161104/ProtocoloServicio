## Copyright (C) 2026 canoj
## Author: canoj <canoj@MSI>
## Created: 2026-03-13

#Condiciones iniciales para la secuencia
a=sigPrimo(20000);
b=sigPrimo(10000);
base=sigPrimo(100000);  #Base debe ser impar (primo)
password =  "pwd";     #contraseña de los dos extremos
x=fix(time);

for i=1:length(password)
  x =(x+uint8(password(i)));  #Pa sumar la contraseña a la llave pública
end

semilla=x; # Guardamos la semilla para que el receptor pueda desencriptar la imagen



imagen = imread("gato.jpg");  #Leemos la imagen
tamano = size(imagen);
vector = imagen(:);       #Convertimos la imagen a un vector por columnas

#encriptando
X=[];
for i=1:length(vector)

  letter = vector(i); # Obtenemos letra por letra de la imagen y

  # Obtener el numero aleatorio y guargarlo en x
  x = mod(a*x+b,base);  # El multiplicador debe de ser impar, mientras mas
                        # diferente de la base es mejor

  xNormalizada = fix(x/base*256); #Normalizamos el numero solamente a 256

  X=[X;bitxor(xNormalizada,letter)]; # Al valor pseudoaleatorio, le sumamos la letra
end


#desencriptando
Y =[];
for i=1:length(X)
  input = X(i); # Obtenemos la letra actual
  semilla = mod(a*semilla+b,base); # Generamos el número aleatorio
  xNormalizada = fix(semilla/base*256);


  pixel = bitxor(input,xNormalizada);

  Y =[Y;pixel];

end


reimagen = reshape(Y, tamano); #Generamos una matriz a vece del vector Y, delimitado por el tamaño
imshow(uint8(reimagen))

