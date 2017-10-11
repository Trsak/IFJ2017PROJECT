# IFJ2017

Semester project of the IFJ (Formal Languages and Translators) of the second year 
of the Master's program at VUT-FIT, Božetěchova 1/2, 612 00 Brno-Královo Pole.

See [fit.vutbr.cz](http://fit.vutbr.cz) for more info.
 
## About the Project

The project is about to complete a functional translator from IFJ17 language to
the IFJcode17 language. The translator has to work as a console application 
(Without a GUI) and returns all the results on standard output (error output).

## Made by 4 units

#### Lexical Analysis (Scanner)

Takes the IFJ17 code and represents each word or character. Creates "Tokens" that
contain information about the attribute. These Tokens are sent to Syntactic 
Analysis.

#### Syntactic Analysis (Parser)

Checks all the information inside the tokens and looks for the syntax errors.
Sends each command to Semantic Analysis.

#### Semantic Analysis

Creates structure and implements the code. It also takes care about all the semantic
problems.

#### Code Generator

Generates the IFJcode17 and prints it to the standard output.

## Authors

* **Bartošek Jan** - [xbarto92](https://github.com/B4rtosek)
* **Bártl Roman** - [xbartl06](https://github.com/romanbartl)
* **Odehnal Tomáš** - [xodehn08](https://github.com/odehnaltomas)
* **Šopf Petr** - [xsopfp00](https://github.com/Trsak)
