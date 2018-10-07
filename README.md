# go-channel-in-cpp

```
$ git clone https://github.com/codesome/go-channel-in-cpp.git
$ cd go-channel-in-cpp/
$ vi test.cpp
// now uncomment the test section that you want
$ make test
```

### Example usage

#### Create channel object
```
Channel<int> my_chan;
```

#### Adding data to channel buffer
```
my_chan.add(30);
```

#### Retrieving data from channel
```
bool is_closed;
int data = my_chan.retrieve(&is_closed);

if(!is_closed) {
    cout << "Data retrieved is " << data << endl;
} else {
    cout << "Channel is closed, so data retrieved is junk value" << endl;
}

```
