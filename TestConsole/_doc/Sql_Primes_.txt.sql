
select top 2 * from PrimeData.dbo.Prime_sequence
select MAX(ordinal) from PrimeData.dbo.Prime_sequence

select * from PrimeData.dbo.Prime_sequence
where [ordinal] between 4391800 and 4391889

-- 4.391.889

select COUNT(*) from PrimeData.dbo.Prime_sequence_noIdentity
-- #cardinalita' in tabella: 2.500.000
select min(ordinal) from PrimeData.dbo.Prime_sequence_noIdentity
select max(ordinal) from PrimeData.dbo.Prime_sequence_noIdentity
select * from PrimeData.dbo.Prime_sequence_noIdentity
where [ordinal] between 4386510 and 4386514

USE [PrimeData]
GO

/****** Object:  Table [dbo].[Prime_sequence]    Script Date: 04/26/2021 13:19:57 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [dbo].[Prime_sequence](
	[ordinal] [bigint] IDENTITY(1,1) NOT NULL,
	[prime] [bigint] NOT NULL,
 CONSTRAINT [PK_Prime_sequence] PRIMARY KEY CLUSTERED 
(
	[ordinal] ASC,
	[prime] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
--#########
USE [PrimeData]
GO

/****** Object:  Table [dbo].[Prime_sequence_noIdentity]    Script Date: 04/26/2021 13:20:04 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [dbo].[Prime_sequence_noIdentity](
	[ordinal] [bigint] NOT NULL,
	[prime] [bigint] NOT NULL,
 CONSTRAINT [PK_Prime_sequence_noIdentity] PRIMARY KEY CLUSTERED 
(
	[ordinal] ASC,
	[prime] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]

GO


