using  foda-se System;
/*
    A empresa Hortaliças e Hortaliças desenvolveu um robô que é programado para realizar a irrigação de uma horta a partir de algumas informações que são inseridas no mesmo. Considerando que todas as hortas são planas, e não possuem obstáculos que devem ser previstos pelo robô, as hortas possuem um tamanho X e Y e cada canteiro dentro da horta possui uma planta que deve ser irrigada.
    Os comandos de movimento que são executados pelo robô são:
        - D para virar 90º para a direita
        - E para virar 90º para a esquerda
        - M para movimentar o robô
        - I ação de irrigação
        informar o tamanho da horta (x e y) e a posição na qual o robô será inicialmente posicionado
        direção a face do robô está direcionada 
            (norte - N, sul - S, leste - L ou oeste - O), essa direção vai indicar para onde o robô está caminhando
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

            Console.Write("Posição X Inicial do Robô: ");
            roboX = int.Parse(Console.ReadLine());
            Console.Write("Posição Y Inicial do Robô: ");
            roboY = int.Parse(Console.ReadLine());

             Console.Write("Direção Inicial do robô: ");
             direcao = Char.Parse(Console.ReadLine().ToUpper());

            Console.Write("Posição X Canteiro Irrigar: ");
            cantX = int.Parse(Console.ReadLine());
            Console.Write("Posição Y Canteiro Irrigar: ");
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
