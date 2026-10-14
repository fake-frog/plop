# Plop is (going to be) a lisp like language 

### syntax ideas

this:
```
(+ 10 (- 20 8))
```

could be writen as this
```
      *---- placeholder character
      |
(+ 10 &) -> (- 20 8)

      *---- numbered placeholder character
      |
(+ 10 &0 &1) -> (- 20 8) (* 255 18)
```
