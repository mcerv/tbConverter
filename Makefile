CC = g++
CFLAGS = -g `root-config --cflags` -O2 -Wall
LFLAGS = `root-config --ldflags --glibs` -O1
OBJPATH = obj
SRCPATH = src
EXECUTABLE = tbConverter
OBJECTS = $(OBJPATH)/configparser.o $(OBJPATH)/filehandler.o $(OBJPATH)/frameworktb.o $(OBJPATH)/inputargs.o $(OBJPATH)/main.o $(OBJPATH)/progbar.o $(OBJPATH)/lecroybin.o $(OBJPATH)/textconvert.o $(OBJPATH)/waveform.o $(OBJPATH)/waveformana.o $(OBJPATH)/hitbus.o $(OBJPATH)/rceconvert.o $(OBJPATH)/rceevent.o $(OBJPATH)/rceplane.o $(OBJPATH)/cluster.o $(OBJPATH)/event.o $(OBJPATH)/hit.o $(OBJPATH)/plane.o $(OBJPATH)/storageio.o $(OBJPATH)/track.o 

all: tbConverter

tbConverter: $(OBJECTS)
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) -o $(EXECUTABLE)

$(OBJPATH)/configparser.o: $(SRCPATH)/configparser.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/configparser.cpp -o $(OBJPATH)/configparser.o

$(OBJPATH)/filehandler.o: $(SRCPATH)/filehandler.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/filehandler.cpp -o $(OBJPATH)/filehandler.o

$(OBJPATH)/frameworktb.o: $(SRCPATH)/frameworktb.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/frameworktb.cpp -o $(OBJPATH)/frameworktb.o

$(OBJPATH)/inputargs.o: $(SRCPATH)/inputargs.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/inputargs.cpp -o $(OBJPATH)/inputargs.o

$(OBJPATH)/main.o: $(SRCPATH)/main.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/main.cpp -o $(OBJPATH)/main.o

$(OBJPATH)/progbar.o: $(SRCPATH)/progbar.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/progbar.cpp -o $(OBJPATH)/progbar.o

$(OBJPATH)/lecroybin.o: $(SRCPATH)/converters/lecroybin.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/converters/lecroybin.cpp -o $(OBJPATH)/lecroybin.o

$(OBJPATH)/textconvert.o: $(SRCPATH)/converters/textconvert.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/converters/textconvert.cpp -o $(OBJPATH)/textconvert.o

$(OBJPATH)/waveform.o: $(SRCPATH)/converters/waveform.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/converters/waveform.cpp -o $(OBJPATH)/waveform.o

$(OBJPATH)/waveformana.o: $(SRCPATH)/converters/waveformana.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/converters/waveformana.cpp -o $(OBJPATH)/waveformana.o

$(OBJPATH)/hitbus.o: $(SRCPATH)/converters/rce/hitbus.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/converters/rce/hitbus.cpp -o $(OBJPATH)/hitbus.o

$(OBJPATH)/rceconvert.o: $(SRCPATH)/converters/rce/rceconvert.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/converters/rce/rceconvert.cpp -o $(OBJPATH)/rceconvert.o

$(OBJPATH)/rceevent.o: $(SRCPATH)/converters/rce/rceevent.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/converters/rce/rceevent.cpp -o $(OBJPATH)/rceevent.o

$(OBJPATH)/rceplane.o: $(SRCPATH)/converters/rce/rceplane.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/converters/rce/rceplane.cpp -o $(OBJPATH)/rceplane.o

$(OBJPATH)/cluster.o: $(SRCPATH)/converters/storage/cluster.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/converters/storage/cluster.cpp -o $(OBJPATH)/cluster.o

$(OBJPATH)/event.o: $(SRCPATH)/converters/storage/event.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/converters/storage/event.cpp -o $(OBJPATH)/event.o

$(OBJPATH)/hit.o: $(SRCPATH)/converters/storage/hit.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/converters/storage/hit.cpp -o $(OBJPATH)/hit.o

$(OBJPATH)/plane.o: $(SRCPATH)/converters/storage/plane.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/converters/storage/plane.cpp -o $(OBJPATH)/plane.o

$(OBJPATH)/storageio.o: $(SRCPATH)/converters/storage/storageio.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/converters/storage/storageio.cpp -o $(OBJPATH)/storageio.o

$(OBJPATH)/track.o: $(SRCPATH)/converters/storage/track.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/converters/storage/track.cpp -o $(OBJPATH)/track.o

clean:
	 rm $(OBJPATH)/*.o tbConverter