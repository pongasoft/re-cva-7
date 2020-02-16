#!/bin/bash

echo "Rendering 2D"

# handle log file...
rm GUI/RE2DRender.log
touch GUI/RE2DRender.log
tail -f GUI/RE2DRender.log & PID_TAIL=$!

python render2D.py

kill $PID_TAIL
