using  foda-se System;
/*
    A empresa Hortali�as e Hortali�as desenvolveu um rob� que � programado para realizar a irriga��o de uma horta a partir de algumas informa��es que s�o inseridas no mesmo. Considerando que todas as hortas s�o planas, e n�o possuem obst�culos que devem ser previstos pelo rob�, as hortas possuem um tamanho X e Y e cada canteiro dentro da horta possui uma planta que deve ser irrigada.
    Os comandos de movimento que s�o executados pelo rob� s�o:
        - D para virar 90� para a direita
        - E para virar 90� para a esquerda
        - M para movimentar o rob�
        - I a��o de irriga��o
        informar o tamanho da horta (x e y) e a posi��o na qual o rob� ser� inicialmente posicionado
        dire��o a face do rob� est� direcionada 
            (norte - N, sul - S, leste - L ou oeste - O), essa dire��o vai indicar para onde o rob� est� caminhando
*/

namespace testeSchaffen
{
    class Program
    {
        static void Main(string[] args)
        {
            int tamX, tamY;
            int roboX, roboY;
            int cantX, cantY;
            char direcao;
            string path = "";

            

            Console.Write("Largura da Horta: ");
            tamX = int.Parse(Console.ReadLine());
            Console.Write("Altura da Horta: ");
            tamY = int.Parse(Console.ReadLine());

            Console.Write("Posi��o X Inicial do Rob�: ");
            roboX = int.Parse(Console.ReadLine());
            Console.Write("Posi��o Y Inicial do Rob�: ");
            roboY = int.Parse(Console.ReadLine());

             Console.Write("Dire��o Inicial do rob�: ");
             direcao = Char.Parse(Console.ReadLine().ToUpper());

            Console.Write("Posi��o X Canteiro Irrigar: ");
            cantX = int.Parse(Console.ReadLine());
            Console.Write("Posi��o Y Canteiro Irrigar: ");
            cantY = int.Parse(Console.ReadLine());


            switch (direcao)
            {
                case 'N':
                        if (roboX < cantX)
                        {
                            path = path + char.ToString('D');
                            while (roboX != cantX)
                            {
                                path = path + char.ToString('M');
                                roboX = roboX + 1;

                            }
                            if (roboY > cantY)
                            {
                                path = path + char.ToString('D');
                                while (roboY != cantY)
                                {
                                    path = path + char.ToString('M');
                                    roboY = roboY - 1;

                                }
                                if ((roboX == cantX) && (roboY == cantY))
                                    {
                                        path = path + char.ToString('I');
                                    }
                            }else{
                                    path = path + char.ToString('E');
                                    while (roboY != cantY)
                                {
                                    path = path + char.ToString('M');
                                    roboY = roboY + 1;

                                }
                                 if ((roboX == cantX) && (roboY == cantY))
                                    {
                                        path = path + char.ToString('I');
                                    }
                                }

                        }else{
                            path = path + char.ToString('E');
                             while (roboX != cantX)
                            {
                                path = path + char.ToString('M');
                                roboX = roboX - 1;

                            }if (roboY < cantY)
                            {
                                path = path + char.ToString('D');
                                while (roboY != cantY)
                                {
                                    path = path + char.ToString('M');
                                    roboY = roboY + 1;

                                }
                                if ((roboX == cantX) && (roboY == cantY))
                                    {
                                        path = path + char.ToString('I');
                                    }
                            }
                        }         
                break;
                case 'S':
                         if (roboX < cantX)
                        {
                            path = path + char.ToString('E');
                            while (roboX != cantX)
                            {
                                path = path + char.ToString('M');
                                roboX = roboX + 1;

                            }
                            if (roboY > cantY)
                            {
                                path = path + char.ToString('D');
                                while (roboY != cantY)
                                {
                                    path = path + char.ToString('M');
                                    roboY = roboY - 1;

                                }
                                if ((roboX == cantX) && (roboY == cantY))
                                    {
                                        path = path + char.ToString('I');
                                    }
                            }else{
                                    path = path + char.ToString('E');
                                     while (roboY != cantY)
                                {
                                    path = path + char.ToString('M');
                                    roboY = roboY + 1;

                                }
                                if ((roboX == cantX) && (roboY == cantY))
                                    {
                                        path = path + char.ToString('I');
                                    }
                                }

                        }else{
                            path = path + char.ToString('D');
                             while (roboX != cantX)
                            {
                                path = path + char.ToString('M');
                                roboX = roboX - 1;

                            }
                            if (roboY < cantY)
                            {
                                path = path + char.ToString('D');
                                 while (roboY != cantY)
                            {
                                path = path + char.ToString('M');
                                roboY = roboY + 1;

                            }
                                
                            }

                            if ((roboX == cantX) && (roboY == cantY))
                                    {
                                        path = path + char.ToString('I');
                                    }
                        }
                break;
                 case 'L':
                         if (roboX < cantX)
                        {
                            path = path + char.ToString('M');
                            roboX = roboX + 1;
                            if (roboY > cantY)
                            {
                                path = path + char.ToString('D');
                                while (roboY != cantY)
                                {
                                    path = path + char.ToString('M');
                                    roboY = roboY - 1;

                                }
                                if ((roboX == cantX) && (roboY == cantY))
                                    {
                                        path = path + char.ToString('I');
                                    }
                            }else{
                                    path = path + char.ToString('E');
                                }

                        }else{
                            path = path + "EE";
                             while (roboX != cantX)
                            {
                                path = path + char.ToString('M');
                                roboX = roboX - 1;

                            }
                             if ((roboX == cantX) && (roboY == cantY))
                                    {
                                        path = path + char.ToString('I');
                                    }
                        }
                break;

                 case 'O':
                         if (roboX > cantX)
                        {
                             path = path + char.ToString('M');
                             roboX = roboX - 1;

                            if (roboY < cantY)
                            {
                                path = path + char.ToString('D');
                                while (roboY != cantY)
                                {
                                    path = path + char.ToString('M');
                                    roboY = roboY + 1;

                                }

                                if ((roboX == cantX) && (roboY == cantY))
                                    {
                                        path = path + char.ToString('I');
                                    }
                            }

                        }
                break;
            
            }

            Console.Write(path);

        }
    }

    
}
