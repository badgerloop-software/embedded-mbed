#!/bin/bash

dir=$(dirname "${BASH_SOURCE[0]}")

chmod +x $dir/pre-commit
cp $dir/pre-commit $dir/.git/hooks/

echo "Copied $dir/pre-commit to $dir/.git/hooks/"
