import 'package:flutter/material.dart';

void main() {
  runApp(
    MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text("My Flutter"),
          centerTitle: true,
        ),
        body: const Text("This is My Homepage"),
      ),
    ),
  );
}
