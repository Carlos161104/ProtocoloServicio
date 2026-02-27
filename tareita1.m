
#Condiciones iniciales para la secuencia
a=sigPrimo(20000);
b=sigPrimo(10000);
base=sigPrimo(100000); #Base debe ser impar (primo)
x=fix(time);

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

  X=[X;bitxor(xNormalizada,letter)] # Al valor pseudoaleatorio, le sumamos la letra en ASCII
end


figure;
plot(X,'.');
