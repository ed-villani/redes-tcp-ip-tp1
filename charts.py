import pandas as pd
import plotly.graph_objects as go
import plotly.figure_factory as ff
import os


def main():
    if not os.path.exists("images"):
        os.mkdir("images")

    datetime_range = [
        'Manhã',
        'Tarde',
        'Noite'
    ]
    periods = [
        'Manhã',
        'Tarde',
        'Noite'
    ]

    universities = [
        'UFRJ',
        'CAM',
        'UCLA',
        'ADELAIDE'
    ]
    for u in universities:
        for range_date, p in zip(datetime_range, periods):
            df = pd.read_excel(f'/Users/eduardovillani/git/redes-tcp-ip-tp1/final_rtt_time.xlsx')
            university = df[df['DESTINY'] == u]
            fig = ff.create_distplot([list(university[p])], [p], histnorm='')
            fig.update_layout(
                title=f"Distribuição dos RRT para a {u.upper()} no perído da {p}",
                xaxis_title="Tempo (ms)",
                yaxis_title="Quantidade"
            )
            fig.update_xaxes(range=[0, max(list(university[p]))*1.1])
            fig.write_image(f"images/fig_{u}_hist_{p}.png")
            fig.show()



if __name__ == '__main__':
    main()
