![Brio Logo](https://brio-lang-static-files.s3-us-west-2.amazonaws.com/brio.png)

Brio Lang is an interpreted, general purpose, object-oriented programming language. Brio Lang is portable and runs on many Unix varients including Linux and macOS, as well as Windows Subsystem for Linux.

## Quick Start
Read the download and usage guide available at [https://www.brio-lang.org](https://www.brio-lang.org).

## Build the Interpreter
First, install the dependencies:
```sh
$ apt-get -y install make g++ libcurl4-openssl-dev libfcgi-dev
```

Next, build the project:
```sh
$ make all
```

## Run the Interpreter

Running Brio Lang without any arguments, or with `-i`, will start the interactive read-eval-print loop.
```sh
$ ./bin/brio -i
```

The Brio Lang REPL can also be launched using Docker.
```sh
$ docker build . -t brio-lang:0.6.0
$ docker run -it brio-lang:0.6.0
```

To execute a Brio Lang program, simply provide the path to your program as an argument.
```sh
$ ./bin/brio ./path/to/code.brio
```

You may use the `--help` argument to see a full list of supported arguments.
```sh
$ ./bin/brio --help
```
```
usage: ./brio [-h] [-v] [-i] [-t] [-gv] [-sym] [-ast] [-fcgi] FILE.brio

optional arguments:
  -h, --help                  Prints the help information
  -v, --version               Prints the version
  -i                          Runs in interactive mode (read-eval-print loop)
  -t                          Prints the tokens from the lexer
  -gv                         Outputs a DOT file for visualizing the AST
  -sym                        Prints the symbol table globals 
  -ast                        Prints each node type in the AST
  -fcgi                       Starts FastCGI listener, must be called from spawn-cgi
```

## Code Examples

### Hello World
```brio
method main(){
    print("Hello world ✋");
};
```
```
$ brio app.brio
Hello world ✋
```

### Fibonnacci Series
```brio
# Fibonacci series up to n
method fib(n){
    let a = 0
    let b = 1
    while (a < n){
        print(a, ',')
        let c = a + b
        a = b
        b = c
    }
}

method main(){
    fib(1000)
}
```
```
$ brio app.brio
0,1,1,2,3,5,8,13,21,34,55,89,144,233,377,610,987
```

### Iterate Over Values
```brio
method main(){
    let numbers = range(5)  # [0, 1, 2, 3, 4]
    let sum = 0

    each (let n : numbers){
        sum += n
    }

    print(sum)
}
```
```
$ brio app.brio
10
```

You can iterate over strings, arrays, dictionaries, as well as user-defined objects.

```brio
method main(){
    let airports = {
        "SFO": {"id": 1, "city": "San Francisco"},
        "FRA": {"id": 2, "city": "Frankfurt"},
        "YOW": {"id": 3, "city": "Ottawa"},
    }

    each (let key : airports){
        print(airports[key]["city"])
    }
}
```
```
$ brio app.brio
San Francisco
Frankfurt
Ottawa
```

## License
Brio Lang is copyrighted free software, licensed under the Modified BSD License. 

See [LICENSE](LICENSE) and [COPYRIGHT](COPYRIGHT) for details.



