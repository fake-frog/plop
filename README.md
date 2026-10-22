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

Or maybe like this?
```
<f32 (+ <f32 10> <i32 $> <u32 $>)>
$(- 20 8) 
$(* 255 18)

(def x <i32 10>)
(type Point <i32 x> <i32 y>)

(fn <Point add-point> (<Point a> <Point b>) (
  <Point ( + a.x b.x ) (+ a.y b.y)>
  ))
  
(fn <Point add-point> (<Point a> <Point b>) (<Point $>))
$((+ a.x b.x) (+ a.y b.y))

(fn <Point add-point> (<Point a> <Point b>) 
    (<Point <i32 @x> <i32 @y>))
    @x (+ a.x b.x)
    @y (+ a.y b.y)
```
