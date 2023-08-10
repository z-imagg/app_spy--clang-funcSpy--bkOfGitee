import numpy
import math

import seaborn

import plotly.offline as plotly_offline
plotly_offline.init_notebook_mode(connected=True)
import plotly.graph_objs as graph_objs
import plotly.figure_factory as figure_factory

from plotly.graph_objs import Scatter, Figure, Scatter3d

import pandas
pandas.options.display.max_columns = None
pandas.options.display.max_rows = None

df_mini_main=pandas.read_csv(filepath_or_buffer="/tick_data_home/clang-15_960162_1691504630415_1",sep=',' , quotechar="'")

# df_mini_main.info()

print(df_mini_main.tail(5).to_string(index=False))

dfmm=df_mini_main

print(dfmm.shape,  df_mini_main.shape)

#3.1 新增列 funcLoc 相当于字符串样式的funId
# 新增列 funcLoc
dfmm['funcLoc']=dfmm .apply(lambda r: f'{r.srcFile}_{r.funcLine}_{r.funcCol}', axis=1 )
print(dfmm['funcLoc'].values[:4])

#3.2 以 区间[0, funcLoc不重复个数-1] 作为 funcId表
funcLoc_values=dfmm['funcLoc'].values
type(funcLoc_values)#numpy.ndarray
len(funcLoc_values)#1014494

funcLoc_list=list(funcLoc_values)
len(funcLoc_list)#1014494

# funcLoc转为集合
funcLoc_set=set(funcLoc_values)
len(funcLoc_set)#2838

# funcLoc集合作为列表: 即 不重复的funcLoc 列表
uqFuncLoc_Ls=list(funcLoc_set)
len(uqFuncLoc_Ls)#2838
# uq:unique

# 不重复的funcLoc 列表 转为 funcId表
funcId_Tab=dict( [(fL,j) for j,fL in enumerate(uqFuncLoc_Ls)] )
print(list(funcId_Tab.items())[:5])

print("funcId个数:",len(funcId_Tab))

#3.3 参照 funLoc列、funcId表 新增列funcId
# 新增列 funcId
dfmm['funcId']=dfmm['funcLoc'] .apply(lambda fLocJ:  funcId_Tab[fLocJ] )
print(dfmm['funcId'].values[:10])
print( dfmm.head(2).to_string(index=False) )

#4. 按funcId分组、组内元素降序
#4.1 按funcId分组、组内元素降序
# 按 funcId 分组
g_fi_dfmm = dfmm.groupby('funcId').size().reset_index(name='count')
# funcId 分组 ，按组内行数降序排列
srt_g_fi_dfmm = g_fi_dfmm.sort_values(by='count', ascending=False)
print( srt_g_fi_dfmm.head(5).to_string(index=False) )

# 按 funcId,funcName 分组.
#   一个funcId 对应 多个funcName，因此funcName是冗余列，只是为了组内能看到funcName的值
g_fiFn_dfmm = dfmm.groupby(['funcId','funcName']).size().reset_index(name='count')
# 按分组内的行数降序排列
srt_g_fiFn_dfmm = g_fiFn_dfmm.sort_values(by='count', ascending=False)
print( srt_g_fiFn_dfmm.head(5).to_string(index=False) )

#4.3 plotly作图
#调用次数前5的函数们
"""
funcIdTop5=srt_g_fiFn_dfmm.funcId.values[:5]
dfmm_sparseFI = dfmm[  dfmm['funcId'].isin( funcIdTop5 )  ]
print(dfmm.shape, len(funcIdTop5), dfmm_sparseFI.shape)

mini_main_sparseFI_栈净_trace = graph_objs.Scatter(
 x = dfmm_sparseFI.滴答,
 y = dfmm_sparseFI.栈净,
 showlegend=True,
mode = 'markers',
marker={'color': dfmm_sparseFI['funcId'],'size':2, 'opacity': 0.9},
name="mini_main_稀疏函数_栈净"
)

plotly_offline.plot([ mini_main_sparseFI_栈净_trace])
"""