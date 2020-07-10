#!/bin/bash

if ! diff a.txt b.txt > diff.txt; then
  echo "a.txt and b.txt differ"
fi
