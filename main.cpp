#include <iostream>
#include <math.h>

using namespace std;

struct TipoCelula {
    int iItem;
    TipoCelula *Prox;
};

template <class T>

class Pilha {
  private:
    T* iContador;
    TipoCelula *Topo;
  public:
    Pilha() {
        Topo = NULL;
        iContador = 0;;
    }
    bool destrutor() {
        T Aux;
        TipoCelula *Temp;
        if( empty() ) return false;
        
        while( Topo != NULL ) {
            Temp = Topo;
            Temp -> Prox = NULL;
            Topo = Topo -> Prox;
            free( Temp );
        }
        iContador = 0;
        return true;
    }
    bool empty() {
        return ( iContador == 0 );
    }
    int tamanho() {
        return iContador;
    }
    void push( T iValor ) {
        TipoCelula *NovaCelula = ( TipoCelula* )malloc( sizeof( TipoCelula ) );
        
        NovaCelula -> iItem = iValor;
        NovaCelula -> Prox = Topo;
        Topo = NovaCelula;
        iContador++;
    }
    T pop() {
        T iValor;
        TipoCelula *Temp;
        if( iContador <= 0 ) throw out_of_range("Pilha vazia");
        iValor = Topo -> iItem;
        Temp = Topo;
        Topo = Topo -> Prox;
        
        free( Temp );
        iContador--;
        return iValor;
    }
    T peek() {
        T iValor; 
        if( iContador <= 0 ) throw out_of_range("Pilha vazia");
        iValor = Topo -> iItem;
        return iValor;
    }
};

bool isDigit( char c ) {
    return ( c >='0' && c <= '9' );
}

bool isOp( char c ) {
    return ( c == '*' || c == '+' || c == '-' || c == '/' || c == '^' || c == '(' || c == ')' );
}

int getPrecedente( char c ) {
    switch (c) {
        case '+':
        case '-': return 1;
        case '*':
        case '/': return 2;
        case '^': return 3;
        case '(': 
        case ')': return 4;
        default: return -1;
    }
}

int operate( int val1, int val2, char op ) {
    if( op == '+') return val1 + val2;
    if( op == '-') return val1 - val2;
    if( op == '*') return val1 * val2;
    if( op == '/') return val1 / val2;
    return pow( val1, val2 );
}

int evaluate ( string s ) {
    Pilha< int > vals;
    Pilha< char > ops;
    
    int val = 0;
    int pos = 0;
    
    while( pos < s.length() ) {
        char spot = s[ pos ];
        if(isDigit( spot ) ) {
            val = ( val * 10 ) + ( int )( spot - '0' );
        } else if( isOp( spot ) ) {
            if( spot == '(' ) {
                ops.push( spot );
                val = 0;
            } else if( vals.empty() ){
                vals.push( val );
                ops.push( spot ); 
                val = 0;
            } else if( spot == ')' ){
                vals.push( val );
                while( ops.peek() != '(' ) {
                    spot = ops.pop();
                    val = vals.pop();
                    int prev = vals.pop();
                    val = operate( prev, val, spot );
                    vals.push( val );
                }
                ops.pop();
                vals.pop();
            } else {
                char prev = ops.peek();
                if( getPrecedente( spot ) > getPrecedente( prev ) ) {
                    vals.push( val );
                    ops.push( spot );
                    val = 0;
                } else {
                    int prevval = vals.pop();
                    int prevop = ops.pop();
                    prevval = operate( prevval, val, prevop );
                    vals.push( prevval );
                    ops.push( spot );
                    val = 0;
                }
            }
        }
        pos++;
    }
    
    while ( !ops.empty()) {
        int prev = vals.pop();
        char spot = ops.pop();
        val = operate( prev, val, spot );
    }
    return val;
}

int main() {
    cout << evaluate("(3+4)*5") << endl;
	return 0;
}
