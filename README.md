# Conceito geral
  O Sistema possui uma lista de produtos e uma lista de moedas. A lista de moedas apenas contem a sua relação com um lastro (que é uma moeda especial, marcada como lastro).

  Os produtos possuem apenas o valor em lastro

  Para tornar o sistema mais 'leve', o valor total em lastro de um produto é realculado quando ele é salvo no banco. O recalculo é necessário pois um produto pode conter, alem do seu proprio valor de lastro, produtos dentro dele.

  então, para que tudo fucione, o produto possui um valor de lastro e um valor total, que é calculado somando-se este lastro com a soma do valor de todos os seus filhos.