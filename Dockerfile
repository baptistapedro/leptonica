#leptonica
FROM fuzzers/afl:2.52

RUN apt-get update
RUN apt install -y install -y build-essential wget git clang cmake  automake autotools-dev  libtool zlib1g zlib1g-dev libexif-dev pkg-config libx11-dev gettext libswscale-dev xzgv libtiff-dev libpng-dev
RUN wget http://www.ijg.org/files/jpegsrc.v9e.tar.gz
RUN tar xvfz jpegsrc.v9e.tar.gz
WORKDIR /jpeg-9e
RUN ./configure CC=afl-clang --prefix=/usr/local/
RUN make
RUN make install
WORKDIR /
RUN git clone https://github.com/DanBloomberg/leptonica.git
WORKDIR /leptonica
COPY file:2cfd565e3d5b3496558da4fa1f68941d24388a364ee346be905039974be5b761 in . 
RUN mkdir ./build
WORKDIR /leptonica/build
RUN cmake -DCMAKE_C_COMPILER=afl-clang -DCMAKE_CXX_COMPILER=afl-clang++ ..
RUN make
RUN make install
WORKDIR /leptonica
RUN clang++ -I/usr/local/include/leptonica fuzz_jpg.cpp  -o /fuzz_jpg /usr/local/lib/libleptonica.a -ljpeg -ltiff -lz -lm -lpng
RUN  mkdir /jpgLeptonicaCorpus
RUN wget https://download.samplelib.com/jpeg/sample-clouds-400x300.jpg
RUN wget https://download.samplelib.com/jpeg/sample-red-400x300.jpg
RUN wget https://download.samplelib.com/jpeg/sample-green-200x200.jpg
RUN wget https://download.samplelib.com/jpeg/sample-green-100x75.jpg
RUN mv *.jpg /jpgLeptonicaCorpus
ENV LD_LIBRARY_PATH=/usr/local/lib


ENTRYPOINT ["afl-fuzz", "-i", "/jpgLeptonicaCorpus", "-o", "/lepjpegOut"]
CMD ["/fuzz_jpg", "@@"]
