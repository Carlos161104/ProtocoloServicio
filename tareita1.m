
#Condiciones iniciales para la secuencia
a=sigPrimo(20000);
b=sigPrimo(10000);
base=sigPrimo(100000); #Base debe ser impar (primo)
x=fix(time);
semilla=x; # Guardamos la semilla para que el receptor pueda desencriptar el mensaje

X=[];


message = 'Esta es una clave que debe de ser encriptada';
largest = length(message);


for i=1:largest

  #Extraer la ultima letra del string
  letter = uint8(message(end)) # Obtenemos la ultima letra del mensaje y lo pasamos a ASCII
  message(end) = []; # Seteamos la ultima posicion

  # Obtener el numero aleatorio y guargarloo en x
  x = mod(a*x+b,base); # El multiplicador debe de ser impar, mientras mas
                      #diferente de la base es mejor

  xNormalizada = fix(x/base*256); #Normalizamos el numero solamente a 256

  X=[X;bitxor(xNormalizada,letter)]; # Al valor pseudoaleatorio, le sumamos la letra en ASCII
end

X
figure;
plot(X,'.');


clc
Y =[];
for i=1:length(X)
  input = X(i); # Obtenemos el primer numero
  semilla = mod(a*semilla+b,base); # Generamos el primer nuemero aleatorio
  xNormalizada = fix(semilla/base*256);


  character = bitxor(input,xNormalizada);

  Y =[Y;char(character)];
end


imagen = imread("gato.jpg");

tamano = size(imagen);
vector = imagen(:);
reimagen = reshape(vector, tamano);
imshow(uint8(reimagen))


