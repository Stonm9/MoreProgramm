FROM python:3.10

RUN pip install notebook
RUN mkdir /workspace
COPY requirements.txt /
RUN pip install -r requirements.txt

RUN useradd -m -s /bin/bash pavel
USER pavel
WORKDIR /home/pavel

EXPOSE 8888

ENTRYPOINT [ "jupyter", "notebook" ]
CMD ["--ip", "0.0.0.0"]