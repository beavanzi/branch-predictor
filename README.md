# Previsor de Desvio de 2 Níveis

Simulador de previsor de desvio de 2 níveis construído com C++ e relatório de análise.

Faça download da ferramenta PIN:

``wget http://software.intel.com/sites/landingpage/pintool/downloads/pin-3.0-76991-gcc-linux.tar.gz``

Descomprima o tarball e mova para a pasta /codigo executando:

``tar xzvf pin-3.0-76991-gcc-linux.tar.gz && mv pin-3.0-76991-gcc-linux codigo/pin && rm pin-3.0-76991-gcc-linux.tar.gz``

Para compilar as variações do código e o relatório, execute:

``make``

Crie as variáveis de ambiente executando:

`export PIN_ROOT=$(pwd)/codigo/pin

export PATH=$PIN_ROOT:$PATH

echo -e "\n# environment variables for branch prediction lab" >> ~/.profile

echo "export PIN_ROOT=$PIN_ROOT"   >> ~/.profile

echo 'export PATH=$PIN_ROOT:$PATH' >> ~/.profile`

Para rodar uma variação, execute:

``make ./codigo/ && pin -t obj-intel64/bp_<variacao>.so -- <programa>``

As variações disponíveis são:
* gag, gas, gap
* pag, pas, pap
* sag, sas, sap

