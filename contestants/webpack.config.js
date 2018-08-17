const webpack = require('webpack')
const path = require('path')
const fs = require('fs')
const UglifyJSPlugin = require('uglifyjs-webpack-plugin')

var nodeModules = {}
fs.readdirSync('node_modules')
  .filter(function(x) {
    return ['.bin'].indexOf(x) === -1;
  })
  .forEach(function(mod) {
    nodeModules[mod] = 'commonjs ' + mod;
  });

module.exports =
{
    entry: './botsubmit.js',
    target: 'node',
    output:
    {
        path: __dirname,
        filename: 'submit.js'
    },
    externals: nodeModules,
    plugins:
    [
        new UglifyJSPlugin(),
    ],
}
