/*
 *   Proyecto M2
 *   Detector Señal SOS
 *   Escrito por Raydelto Hernández
 *   Fecha Creación: Viernes 3 de Marzo 2006
 *   Ultima Revisión:  Lunes 24 de Abril 2006
 *   Version 0.1 Alpha
 */

#device pic16f877
#use delay(clock = 4000000)
#byte puertoB = 6
#bit pb0 = puertoB .0
#bit pb1 = puertoB .1
#bit pb2 = puertoB .2
#bit pb3 = puertoB .3
#bit pb4 = puertoB .4
#bit pb5 = puertoB .5
#bit pb6 = puertoB .6
#bit pb7 = puertoB .7

unsigned sos[17] = {25, 25, 25, 25, 25, 50, 50, 25, 50, 25, 50, 50, 25, 25, 25, 25, 25};
unsigned *puertoA = 5;

long ms = 0;             // milisegundos transcurridos
unsigned tolerancia = 5; // tolerancia de error
unsigned senalCorrecta[8] = {0, 0, 0, 0, 0, 0, 0, 0};

int verificar()
{
  unsigned i = 0; // indice del arreglo de SOS
  unsigned k = 0; // Indice del Fototransistor a Analizar
  unsigned tmpB = 0;

  ms = 0;

  while (i < 16)
  {

    if ((*puertoB != tmpB))
    {

      if ((sos[i] >= (ms - tolerancia)) && (sos[i] <= (ms + tolerancia)))
      {
        senalCorrecta[k] = 1;
        i++;
        ms = 0;
      }

      else
      {
        senalCorrecta[k] = 0;

        break;
      }
    }

    tmpB = *puertoB;
    delay_ms(5);
    ms += 5;
  }

  return senalCorrecta;
}

int analizar()
{

  ms = 0;

  // Detectar el inicio de la señal
  while (1)
  {

    if (*puertoA == 0)
    {
      ms += 5;
    }
    else
    {
      ms = 0;
    }

    if (ms >= 995)
    {

      return verificar();
    }

    delay_ms(5);
  }
}

void main()
{
  set_tris_a(0x3F); // Configurando Puerto A, todos los Pines son de entrada
  set_tris_b(0x3F); // Configurando Puerto B, todos los Pines son de salida.
  *puertoB = 0;

  while (1)
  {
    if (*puertoA != 0)
    {

      if (analizar())
      {
        *puertoB = 255;
        break;
      }

      else
      {
        *puertoB = 2;
      }
    }
  }
}
